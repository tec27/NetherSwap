#ifndef HOOK_INJECTOR_H
#define HOOK_INJECTOR_H

#include <Windows.h>

typedef BOOL (WINAPI *ClipCursorFunc)(const RECT *lpRect);

struct InjecteeFunctionTable {
  HMODULE (WINAPI* LoadLibrary)(LPCWSTR lpLibFileName);
  HMODULE (WINAPI* GetModuleHandle)(LPCWSTR lpModuleName);
  BOOL (WINAPI* FreeLibrary)(HMODULE hModule);
  FARPROC (WINAPI* GetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
  BOOL (WINAPI* VirtualProtect)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect,
      PDWORD lpflOldProtect);
  HANDLE (WINAPI* CreateFile)(LPCWSTR lpFileName, DWORD dwDesiredAccess,
      DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
      DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
  BOOL (WINAPI* CloseHandle)(HANDLE hObject);
  BOOL (WINAPI* WriteFile)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
      LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
};

struct StringTable {
  wchar_t user32_dll[11];
  char clip_cursor[11];
  wchar_t pipe_name[23];
};

struct HookInfo {
  ClipCursorFunc orig_clipcursor;
  byte orig_head[6];
  byte new_head[6];
  
};

struct InjecteeInfo {
  InjecteeFunctionTable functions;
  StringTable string_table;
  ClipCursorFunc clipcursor_hook;
  HookInfo hook_info;
};

class HookInjector {
public:
  explicit HookInjector(DWORD process_id);
  bool Inject();
private:
  DWORD process_id_;
};

#endif
