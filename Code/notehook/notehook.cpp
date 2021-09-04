#include "framework.h"
#include "notehook.h"

typedef unsigned char byte;

HINSTANCE g_hInstance = NULL;
HHOOK g_Hook = NULL;
HANDLE hFileMapT = INVALID_HANDLE_VALUE;
HANDLE hEvent = INVALID_HANDLE_VALUE;

#define HOST_PROCESS "flashnote.exe"
#define WHITE_LIST "WeChat.exe"
#define FLOAT_WIN_CLASS L"Qt5150dQWindowIcon"
#define FLOAT_WIN_NAME L"floating_window"

DWORD buffSize = 1024;
TCHAR ProcessName[1024];
POINT GlobalP;

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0)
	{
		//TODO����ϼ�������
		WORD vkCode = LOWORD(wParam);
		BOOL altDownFlag = (HIWORD(lParam) & KF_ALTDOWN);
		if (altDownFlag && vkCode == 'S')
		{
			GetCursorPos(&GlobalP);
			//HWND hwnd = WindowFromPoint(p);
			//SendMessage(hwnd, WM_COPY, 0, 0);		��ʱ�޷�����Ϣ����֪ͨ����

			Sleep(100);
			INPUT* input = new INPUT[4];
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
			SendInput(4, input, sizeof(INPUT));

			HWND hwnd = FindWindowW(FLOAT_WIN_CLASS, FLOAT_WIN_NAME);
			if (false && hwnd)
			{
				COPYDATASTRUCT data;
				data.dwData = 0;
				data.cbData = sizeof(GlobalP);
				data.lpData = &GlobalP;
				//TODO: ��ʵҲ���԰Ѽ�����֮ǰ�����ݷ���ȥ
				LRESULT ret = SendMessage(hwnd, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)(LPVOID)&data);
				DWORD lastErr = GetLastError();
			}
			if (true)
			{
				if (hFileMapT == INVALID_HANDLE_VALUE)
					hFileMapT = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, TEXT("FlashMousePosition"));
				if (hEvent == INVALID_HANDLE_VALUE)
					hEvent = OpenEventW(SYNCHRONIZE, FALSE, L"FlashMouseEvnet");
				if (hFileMapT != INVALID_HANDLE_VALUE && hEvent != INVALID_HANDLE_VALUE)
				{
                    PVOID pView = MapViewOfFile(hFileMapT, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
					if (pView != NULL) {
						memcpy(pView, &GlobalP, sizeof(GlobalP));
						//SetEvent(hEvent);
						UnmapViewOfFile(pView);
					}
				}
			}
		}
	}
	return CallNextHookEx(g_Hook, code, wParam, lParam);
}

void installHook()
{
	g_Hook = SetWindowsHookEx(WH_KEYBOARD, GetMsgProc, g_hInstance, 0);
}

void uninstallHook()
{
	UnhookWindowsHookEx(g_Hook);
}

BOOL hook_by_code(LPCSTR szDllName, LPCSTR szFuncName, PROC pfnNew, PBYTE pOrgBytes)
{
	FARPROC pfnOrg;
	DWORD dwOldProtect, dwAddress;
	byte pBuf[5] = { 0xE9, 0, };
	PBYTE pByte;

	//��ȡҪ��ȡ��API��ַ
	pfnOrg = (FARPROC)GetProcAddress(GetModuleHandleA(szDllName), szFuncName);
	pByte = (PBYTE)pfnOrg;

	//���Ѿ�����ȡ���򷵻�FALSE
	if (pByte[0] == 0xE9)
		return FALSE;

	// Ϊ���޸�5���ֽڣ������ڴ���ӡ�д������
	VirtualProtect((LPVOID)pfnOrg, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	//����ԭ�д��루5�ֽڣ�
	memcpy(pOrgBytes, pfnOrg, 5);

	// ����JMP��ַ
	// XXXXXX = (DWORD)pfnNew - (DWORD)pfnOrg - 5;
	dwAddress = (DWORD)pfnNew - (DWORD)pfnOrg - 5;
	memcpy(&pBuf[1], &dwAddress, 4);

	// �����ӡ����޸�5���ֽڣ�JMP XXXXX)
	memcpy(pfnOrg, pBuf, 5);

	// �ָ��ڴ�����
	VirtualProtect((LPVOID)pfnOrg, 5, dwOldProtect, &dwOldProtect);
	return TRUE;
}

BOOL unhook_by_code(LPCSTR szDllName, LPCSTR szFuncName, PBYTE pOrgBytes)
{
	FARPROC pFunc;
	DWORD dwOldProtect;
	PBYTE pByte;

	//��ȡAPI��ַ
	pFunc = GetProcAddress(GetModuleHandleA(szDllName), szFuncName);
	pByte = (PBYTE)pFunc;

	//���Ѿ����ѹ������򷵻�FALSE
	if (pByte[0] != 0xE9)
		return FALSE;

	VirtualProtect((LPVOID)pFunc, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// �ѹ�
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

	g_hInstance = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		{
			if (hFileMapT != INVALID_HANDLE_VALUE)
			{
				CloseHandle(hFileMapT);
				hFileMapT = INVALID_HANDLE_VALUE;
			}
		}
		break;
	}
	return TRUE;
}