#include <Windows.h>
#include "uiapplication.h"
#include "guihelper.h"

HHOOK gLocalHook = NULL;

static POINT GlobalP;
static POINT mouseDownPos, mouseUpPos, mouseDblClickPos;
static bool bSendedCp = false;

#define NUM_KEYS 4


struct EXTRACT_INFO_
{
	WCHAR text[4096 + 1];
	POINT p;
	DWORD dwProcessId;
};

INPUT* Generate_Ctrl_C()
{
	INPUT* input = new INPUT[NUM_KEYS];
	if (input == NULL)
		return NULL;
	for (int i = 0; i < 4; i++)
	{
		input[i].type = INPUT_KEYBOARD;
		input[i].ki.wScan = 0;
		input[i].ki.time = 0;
		input[i].ki.dwExtraInfo = 0;
	}

	// Press the "Ctrl" key
	input[0].ki.wVk = VK_CONTROL;
	input[0].ki.dwFlags = 0;

	// Press the "C" key
	input[1].ki.wVk = 'C';
	input[1].ki.dwFlags = 0;

	// Release the "C" key
	input[2].ki.wVk = 'C';
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;

	// Release the "Ctrl" key
	input[3].ki.wVk = VK_CONTROL;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	return input;
}

LRESULT CALLBACK MouseMsgProcLL_(int nCode, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(gLocalHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseMsgProcLL(int code, WPARAM wParam, LPARAM lParam) {
	if (code < 0)
		return CallNextHookEx(gLocalHook, code, wParam, lParam);

	PMOUSEHOOKSTRUCT pHookStruct = (PMOUSEHOOKSTRUCT)lParam;
	switch (wParam)
	{
	case WM_MOUSEMOVE:
	{
		if (bSendedCp)
		{
			bSendedCp = false;

			EXTRACT_INFO_ info;
			int len = 0;
			if (OpenClipboard(NULL))
			{
				HANDLE hData = GetClipboardData(CF_UNICODETEXT);
				if (hData != NULL)
				{
					WCHAR* pszText = static_cast<WCHAR*>(GlobalLock(hData));
					if (pszText)
					{
						len = wcslen(pszText);
						wcscpy(info.text, pszText);
					}
					GlobalUnlock(hData);
				}
			}
			info.text[len] = '\0';
			info.p = mouseUpPos;
			info.dwProcessId = GetCurrentProcessId();
			EmptyClipboard();
			CloseClipboard();

			if (wcslen(info.text) > 0) {
				QString qsStr = QString::fromStdWString(info.text);
				AppHelper::uiApp()->captureText(qsStr, QPoint(info.p.x, info.p.y));
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		mouseDownPos = pHookStruct->pt;
		break;
	}
	case WM_LBUTTONUP:
	{
		mouseUpPos = pHookStruct->pt;
		INPUT* input = Generate_Ctrl_C();
		if (input == NULL)
			break;
		SendInput(NUM_KEYS, input, sizeof(INPUT));
		bSendedCp = true;
		break;
	}
	case WM_LBUTTONDBLCLK:
	{
		break;
	}
	}
	return CallNextHookEx(gLocalHook, code, wParam, lParam);
}