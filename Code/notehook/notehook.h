#ifndef __NOTE_HOOK_H__
#define __NOTE_HOOK_H__

extern "C" {
	__declspec(dllexport) void installHook();
	__declspec(dllexport) void uninstallHook();
}

#define MAX_EXTRACT_LENGTH 4096
struct EXTRACT_INFO
{
    WCHAR text[MAX_EXTRACT_LENGTH + 1];
    POINT p;
    DWORD dwProcessId;
};

#endif