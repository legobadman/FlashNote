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

#define ENABLE_MOUSE_HOOK

#define HOST_PROCESS "flashnote.exe"
#define WHITE_LIST "WeChat.exe"
//#define FLOAT_WIN_CLASS L"Qt5150dQWindowIcon"
#define FLOAT_WIN_CLASS L"Qt5150QWindowIcon"
#define FLOAT_WIN_NAME L"笔记"	//如果窗口标题改了就要同步这个

DWORD buffSize = 1024;
TCHAR ProcessName[1024];
POINT GlobalP;
POINT mouseDownPos, mouseUpPos, mouseDblClickPos;
bool bSendedCp = false;
#define NUM_KEYS 4


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

char* GetModuleFileNameOnly()
{
    TCHAR szFileName[MAX_PATH];
    GetModuleFileName(NULL, szFileName, MAX_PATH);
    char* p = strrchr(szFileName, '\\');
    if (p != 0)
        return p + 1;
    else
        return 0;
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
				HWND hwnd = FindWindowW(FLOAT_WIN_CLASS, FLOAT_WIN_NAME);
				if (!hwnd)
					break;

				//chrome会被影响，不知为何
                //if (p && strcmp(p + 1, "flashnote.exe") == 0)
                //    break;

				EXTRACT_INFO info;
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

                COPYDATASTRUCT data;
                data.dwData = 0;
                data.cbData = sizeof(EXTRACT_INFO);
                data.lpData = &info;
                SendMessage(hwnd, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)(LPVOID)&data);
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


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(g_msgHook, nCode, wParam, lParam);;
}

void installHook()
{
	//g_msgHook = SetWindowsHookEx(WH_MOUSE, MouseMsgProc, g_hInstance, 0);
	g_msgHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);
}

void uninstallHook()
{
	UnhookWindowsHookEx(g_msgHook);
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
	//GetModuleFileName(NULL, ProcessName, buffSize);
	//p = strrchr(ProcessName, '\\');

	//方便调试，但好像后续的程序都没有加载
	//if (p && strcmp(p + 1, "devenv.exe") == 0)
		//return FALSE;

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