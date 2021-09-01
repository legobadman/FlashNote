#ifndef __NOTE_HOOK_H__
#define __NOTE_HOOK_H__

extern "C" {
	__declspec(dllexport) void installHook();
	__declspec(dllexport) void uninstallHook();
}

#endif