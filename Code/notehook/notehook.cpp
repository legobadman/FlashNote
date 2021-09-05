#include "framework.h"
#include "notehook.h"
#include <uiautomation.h>
#include <codecvt>
#include <string>

typedef unsigned char byte;

HINSTANCE g_hInstance = NULL;
HHOOK g_KeyboardHook = NULL;
HHOOK g_msgHook = NULL;
HANDLE hFileMapT = INVALID_HANDLE_VALUE;
HANDLE hEvent = INVALID_HANDLE_VALUE;
char* p = NULL;


//#define ENABLE_KEYBOARD_HOOK
#define ENABLE_MOUSE_HOOK

#define MAX_EXTRACT_LENGTH 1024
#define HOST_PROCESS "flashnote.exe"
#define WHITE_LIST "WeChat.exe"
#define FLOAT_WIN_CLASS L"Qt5150dQWindowIcon"
#define FLOAT_WIN_NAME L"我的笔记本"

DWORD buffSize = 1024;
TCHAR ProcessName[1024];
POINT GlobalP;
POINT mouseDownPos, mouseUpPos, mouseDblClickPos;
bool bSendedCp = false;
#define NUM_KEYS 4

struct EXTRACT_INFO
{
	WCHAR text[MAX_EXTRACT_LENGTH];
	POINT p;
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

HRESULT FindTextPatternElement(IUIAutomation* automation, _In_ IUIAutomationElement* element, _Outptr_result_maybenull_ IUIAutomationElement** textElement)
{
    HRESULT hr = S_OK;

    // Create a condition that will be true for anything that supports Text Pattern 2
    IUIAutomationCondition* textPatternCondition;
    VARIANT trueVar;
    trueVar.vt = VT_BOOL;
    trueVar.boolVal = VARIANT_TRUE;
    hr = automation->CreatePropertyCondition(UIA_IsTextPattern2AvailablePropertyId, trueVar, &textPatternCondition);

    if (FAILED(hr))
    {
        wprintf(L"Failed to CreatePropertyCondition, HR: 0x%08x\n", hr);
    }
    else
    {
        // Actually do the search
        hr = element->FindFirst(TreeScope_Subtree, textPatternCondition, textElement);
        if (FAILED(hr))
        {
            wprintf(L"FindFirst failed, HR: 0x%08x\n", hr);
        }
        else if (*textElement == NULL)
        {
            wprintf(L"No element supporting TextPattern2 found.\n");
            hr = E_FAIL;
        }
        textPatternCondition->Release();
    }

    return hr;
}

LRESULT CALLBACK MouseMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
		return CallNextHookEx(g_msgHook, code, wParam, lParam);

	PMOUSEHOOKSTRUCT pHookStruct = (PMOUSEHOOKSTRUCT)lParam;
	switch (wParam)
	{
		case WM_MOUSEMOVE:
		{
			if (bSendedCp)
			{
				bSendedCp = false;
                if (!OpenClipboard(NULL))
                    break;

                HANDLE hData = GetClipboardData(CF_UNICODETEXT);
                if (hData == NULL)
                    break;
                WCHAR* pszText = static_cast<WCHAR*>(GlobalLock(hData));
                if (pszText == nullptr)
                    break;
				int len = wcslen(pszText);
                GlobalUnlock(hData);
                CloseClipboard();

                HWND hwnd = FindWindowW(FLOAT_WIN_CLASS, FLOAT_WIN_NAME);
                if (hwnd && len > 0)
                {
					EXTRACT_INFO info;
					info.p = mouseDownPos;
					wcscpy(info.text, pszText);
					info.text[len] = '\0';

                    COPYDATASTRUCT data;
                    data.dwData = 0;
                    data.cbData = sizeof(EXTRACT_INFO);
                    data.lpData = &info;
                    LRESULT ret = SendMessage(hwnd, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)(LPVOID)&data);
                    return 0;
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
			//if (p == NULL || _stricmp(p + 1, WHITE_LIST) != 0) break;
			mouseUpPos = pHookStruct->pt;
			int dist = std::pow(mouseUpPos.x - mouseDownPos.x, 2) + std::pow(mouseUpPos.y - mouseDownPos.y, 2);
			if (dist > 16)
			{
                INPUT* input = Generate_Ctrl_C();
				if (input == NULL)
					break;
                SendInput(NUM_KEYS, input, sizeof(INPUT));
				bSendedCp = true;
			}
			break;
		}
		case WM_LBUTTONDBLCLK:
		{
			break;
		}
	}

	return CallNextHookEx(g_msgHook, code, wParam, lParam);
}

LRESULT CALLBACK MouseMsgProc2(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
		return CallNextHookEx(g_msgHook, code, wParam, lParam);

	PMOUSEHOOKSTRUCT pHookStruct = (PMOUSEHOOKSTRUCT)lParam;
	switch (wParam)
	{
		case WM_LBUTTONDOWN:
		{
			mouseDownPos = pHookStruct->pt;
			break;
		}
		case WM_LBUTTONUP:
		{
			mouseUpPos = pHookStruct->pt;
			int dist = std::pow(mouseUpPos.x - mouseDownPos.x, 2) + std::pow(mouseUpPos.y - mouseDownPos.y, 2);
			if (dist > 16)
			{
				HRESULT hr = CoInitialize(NULL);
				if (FAILED(hr))
					break;

                IUIAutomation* _automation = NULL;
                hr = CoCreateInstance(__uuidof(CUIAutomation8), NULL,
                    CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_automation));
				if (FAILED(hr) || _automation == NULL)
					break;

				IUIAutomationElement* element = NULL;
				hr = _automation->ElementFromPoint(mouseUpPos, &element);
				if (SUCCEEDED(hr) && element != NULL)
				{
                    IUIAutomationElement* textElement = NULL;
                    hr = FindTextPatternElement(_automation, element, &textElement);
                    if (SUCCEEDED(hr) && textElement != NULL)
                    {
						IUIAutomationTextPattern2* textPattern = NULL;
						hr = textElement->GetCurrentPatternAs(UIA_TextPattern2Id, IID_PPV_ARGS(&textPattern));
						if (SUCCEEDED(hr) && textPattern)
						{
							IUIAutomationTextRangeArray* uiaRangeArray = NULL;
							hr = textPattern->GetSelection(&uiaRangeArray);
							if (FAILED(hr) || uiaRangeArray == NULL)
								break;

                            int length = -1;
                            hr = uiaRangeArray->get_Length(&length);
                            if (!SUCCEEDED(hr) || length <= 0)
								break;

							IUIAutomationTextRange* uiaRange = NULL;
							hr = uiaRangeArray->GetElement(0, &uiaRange);
							if (!SUCCEEDED(hr) || uiaRange == NULL)
								break;
							uiaRangeArray->Release();

                            BSTR text;
                            HRESULT hr = uiaRange->GetText(10000, &text);
							if (FAILED(hr))
								break;

                            HWND hwnd = FindWindowW(FLOAT_WIN_CLASS, FLOAT_WIN_NAME);
                            if (hwnd)
                            {
								std::wstring ws(text, SysStringLen(text));

                                COPYDATASTRUCT data;
                                data.dwData = 0;
                                data.cbData = sizeof(std::wstring);
                                data.lpData = &ws;

                                LRESULT ret = SendMessage(hwnd, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)(LPVOID)&data);
                                return 0;
                            }
						}
                    }
					element->Release();
				}
				_automation->Release();
				CoUninitialize();
			}
			break;
		}
	}
	return CallNextHookEx(g_msgHook, code, wParam, lParam);
}

void installHook()
{
#ifdef ENABLE_KEYBOARD_HOOK
	g_KeyboardHook = SetWindowsHookEx(WH_KEYBOARD, keyboarMsgProc, g_hInstance, 0);
#endif
#ifdef ENABLE_MOUSE_HOOK
	g_msgHook = SetWindowsHookEx(WH_MOUSE, MouseMsgProc, g_hInstance, 0);
#endif
}

void uninstallHook()
{
#ifdef ENABLE_KEYBOARD_HOOK
	UnhookWindowsHookEx(g_KeyboardHook);
#endif
#ifdef ENABLE_MOUSE_HOOK
	UnhookWindowsHookEx(g_msgHook);

#endif
}

BOOL hook_by_code(LPCSTR szDllName, LPCSTR szFuncName, PROC pfnNew, PBYTE pOrgBytes)
{
	FARPROC pfnOrg;
	DWORD dwOldProtect, dwAddress;
	byte pBuf[5] = { 0xE9, 0, };
	PBYTE pByte;

	//获取要钩取的API地址
	pfnOrg = (FARPROC)GetProcAddress(GetModuleHandleA(szDllName), szFuncName);
	pByte = (PBYTE)pfnOrg;

	//若已经被钩取，则返回FALSE
	if (pByte[0] == 0xE9)
		return FALSE;

	// 为了修改5个字节，先向内存添加“写”属性
	VirtualProtect((LPVOID)pfnOrg, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	//备份原有代码（5字节）
	memcpy(pOrgBytes, pfnOrg, 5);

	// 计算JMP地址
	// XXXXXX = (DWORD)pfnNew - (DWORD)pfnOrg - 5;
	dwAddress = (DWORD)pfnNew - (DWORD)pfnOrg - 5;
	memcpy(&pBuf[1], &dwAddress, 4);

	// “钩子”：修改5个字节（JMP XXXXX)
	memcpy(pfnOrg, pBuf, 5);

	// 恢复内存属性
	VirtualProtect((LPVOID)pfnOrg, 5, dwOldProtect, &dwOldProtect);
	return TRUE;
}

BOOL unhook_by_code(LPCSTR szDllName, LPCSTR szFuncName, PBYTE pOrgBytes)
{
	FARPROC pFunc;
	DWORD dwOldProtect;
	PBYTE pByte;

	//获取API地址
	pFunc = GetProcAddress(GetModuleHandleA(szDllName), szFuncName);
	pByte = (PBYTE)pFunc;

	//若已经“脱钩”，则返回FALSE
	if (pByte[0] != 0xE9)
		return FALSE;

	VirtualProtect((LPVOID)pFunc, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// 脱钩
	memcpy(pFunc, pOrgBytes, 5);
	VirtualProtect((LPVOID)pFunc, 5, dwOldProtect, &dwOldProtect);
	return TRUE;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	GetModuleFileName(NULL, ProcessName, buffSize);
	//p = strrchr(ProcessName, '\\');

	g_hInstance = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}