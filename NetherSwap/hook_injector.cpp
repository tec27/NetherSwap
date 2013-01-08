#include "hook_injector.h"
#include <Windows.h>

#pragma runtime_checks("", off)

// This is what actually gets injected in. We copy this function
// over byte-for-byte and run it in the remote process. Since it
// won't have access to our DLLs or memory, we need to give it the
// ability to retrieve all the function addresses/DLLs it will need.
DWORD WINAPI RemoteThreadProc(InjecteeInfo* info) {

  InjecteeFunctionTable& funcs = info->functions;
  HookInfo& hook = info->hook_info;
  HMODULE user32 = funcs.GetModuleHandle(info->string_table.user32_dll);
  hook.orig_clipcursor = reinterpret_cast<ClipCursorFunc>(
      funcs.GetProcAddress(user32, info->string_table.clip_cursor));

  // Initialize hook data (back up current memory, create new memory values)
  DWORD old_protection;
  if(!funcs.VirtualProtect(hook.orig_clipcursor, 6, PAGE_EXECUTE_READWRITE,
      &old_protection)) {
    return -1;
  }

  // build a push <hook func addr>; ret; opcode set
  ClipCursorFunc* func_addr_ptr = reinterpret_cast<ClipCursorFunc*>(&hook.new_head[1]);
  *func_addr_ptr = info->clipcursor_hook;
  hook.new_head[0] = 0x68; // push
  hook.new_head[5] = 0xC3; // ret

  byte* func_memory = reinterpret_cast<byte*>(hook.orig_clipcursor);
  // back up and hook in the same loop
  for(int i = 0; i < 6; i++) {
    hook.orig_head[i] = func_memory[i];
    func_memory[i] = hook.new_head[i];
  }

  if(!funcs.VirtualProtect(hook.orig_clipcursor, 6, old_protection, &old_protection)) {
    return -2;
  }

  return 0;
}

void RemoteThreadProcEnd() {
}

BOOL WINAPI ClipCursorHook(const RECT *lpRect) {
  // This is set to a magic number here, should get replaced with the actual
  // address by the injector
  const InjecteeInfo* info = reinterpret_cast<InjecteeInfo*>('HOON');
  const InjecteeFunctionTable& funcs = info->functions;
  const HookInfo& hook = info->hook_info;

  // restore original memory and re-call ClipCursor with the parameters given
  DWORD old_protection;
  if(!funcs.VirtualProtect(hook.orig_clipcursor, 6, PAGE_EXECUTE_READWRITE,
      &old_protection)) {
    return false;
  }
  byte* func_memory = reinterpret_cast<byte*>(hook.orig_clipcursor);
  for(int i = 0; i < 6; i++) {
    func_memory[i] = hook.orig_head[i];
  }

  BOOL result = hook.orig_clipcursor(lpRect);

  if(result) {
    // we only want to try to emit status changes on successful ClipCursor calls
    // ClipCursor(NULL) disables clipping, so this happens when moving out of a game
    bool in_game = (lpRect != NULL);
    // open named pipe to communicate with netherswap process
    HANDLE pipe = funcs.CreateFile(info->string_table.pipe_name, GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, NULL, NULL);

    if(pipe != INVALID_HANDLE_VALUE) { 
      byte data[2]; // QLocalServer likes to get null-terminated data for some reason
      data[0] = in_game ? 0x01 : 0x00;
      data[1] = 0x00;

      DWORD bytes_written;
      funcs.WriteFile(pipe, data, sizeof(byte)*2, &bytes_written, NULL);
      funcs.CloseHandle(pipe);
    }
    else {
      // invalid pipe handle likely means NetherSwap is no longer running
      // just don't re-hook
      return result; 
    }
  }

  // restore hook
  for(int i = 0; i < 6; i++) {
    func_memory[i] = hook.new_head[i];
  }
  funcs.VirtualProtect(hook.orig_clipcursor, 6, old_protection, &old_protection);

  return result;
}

void ClipCursorHookEnd() {
}

#pragma runtime_checks("", restore)

byte* GetFunctionBytes(const void* function_start, const void* function_end, size_t& out_length) {
  wchar_t msg[256];
  out_length = reinterpret_cast<size_t>(function_end) - reinterpret_cast<size_t>(function_start);
  byte* function_bytes = new byte[out_length];
  memcpy(function_bytes, function_start, out_length);

  return function_bytes;
}

// Replaces a single instance of magic_bytes in the function_bytes
// Works most efficiently if magic_bytes contains no repeated values, but works fine either way
// replacement_bytes is assumed to be the same length as magic_bytes here
bool ReplaceMagicBytes(byte* function_bytes, const size_t function_length, const byte* magic_bytes,
    const size_t magic_bytes_length, const byte* replacement_bytes) {
  // first we construct a table that says how much to jump ahead/back by for any given byte value
  int jump_by[256];
  // for most values (assuming magic_bytes contains few characters), we can skip
  // MB_length bytes
  for(int i = 0; i < 256; i++) {
    jump_by[i] = magic_bytes_length;
  }
  // then for each byte value in MB we can calculate the actual skip value (may be negative)
  // except for the first byte value, which will make more sense later
  for(size_t i = 1; i < magic_bytes_length; i++) {
    jump_by[magic_bytes[i]] = -i;
  }

  byte magic_start = magic_bytes[0];
  bool matching = false;
  size_t matching_index = 0;

  // Loop through the bytes, skipping as many as possible on each iteration.
  // Whenever we find the starting byte of magic_bytes, we go into a matching mode
  // and move forward 1-by-1 until we either match all the magic_bytes, or find a
  // non-matching byte. If the latter, we go back into skip-mode, otherwise we
  // replace and return.
  for(size_t i = 0; i < function_length;) {
    if(!matching) {
      if(function_bytes[i] != magic_start) {
        // not the start of our magic bytes, jump by as much as possible
        int jump = jump_by[function_bytes[i]];
        if(i+jump < 0) jump = magic_bytes_length;
        i += jump;
      }
      else {
        // may have found the first magic byte
        matching = true;
        matching_index = 1;
        i += 1;
      }
    }
    else {
      if(matching_index == magic_bytes_length) {
        // we found a match! do the replacement and return
        for(size_t j = i - magic_bytes_length, x = 0; j < i; j++, x++) {
          function_bytes[j] = replacement_bytes[x];
        }
        return true;
      }
      else if(function_bytes[i] != magic_bytes[matching_index]) {
        // this is not our magic bytes location
        matching = false;
        i += magic_bytes_length;
      }
      else {
        // still a match, continue on
        matching_index += 1;
        i += 1;
      }
    }
  }

  return false;
}

void InitInjecteeFunctionTable(InjecteeFunctionTable& table) {
  // these functions are all in DLLs with static locations so the addresses from this program
  // can be safely used in the other
  table.LoadLibrary = LoadLibrary;
  table.GetModuleHandle = GetModuleHandle;
  table.FreeLibrary = FreeLibrary;
  table.GetProcAddress = GetProcAddress;
  table.VirtualProtect = VirtualProtect;
  table.CreateFile = CreateFile;
  table.CloseHandle = CloseHandle;
  table.WriteFile = WriteFile;
}

void InitStringTable(StringTable& table) {
  wsprintf(table.user32_dll, L"user32.dll");
  wsprintfA(table.clip_cursor, "ClipCursor");
  wsprintf(table.pipe_name, L"\\\\.\\pipe\\netherswap_ipc");
}

bool InjectFunctionData(const HANDLE process_handle, byte* data, const size_t data_size,
    void** remote_location) {
  // TODO(tec27): some better error handling/logging here would be nice
  void* remote_memory = VirtualAllocEx(process_handle, NULL, data_size, MEM_COMMIT,
      PAGE_EXECUTE_READWRITE);
  if(remote_memory == NULL) {
    return false;
  }

  SIZE_T bytes_written;
  bool success = WriteProcessMemory(process_handle, remote_memory, data, data_size, 
    &bytes_written);
  if(!success || bytes_written != data_size) {
    return false;
  }
  *remote_location = remote_memory;

  return true;
}

// this allows us to more easily clean up memory on errors without resorting to goto's
bool InjectProcAndHook(const HANDLE process_handle, byte* remote_proc_bytes, 
  const size_t remote_proc_size, byte* clipcursor_hook_bytes, const size_t clipcursor_hook_size) {
  // There's a circular dependency between InjecteeInfo and the ClipCursor hook
  // function, so we need to inject one of them slightly differently. I've chosen
  // InjecteeInfo because its not a function. It becomes a 2-part injection:
  // 1) Allocate and store location
  //    1A) Replace magic bytes in hook function with location
  //    1B) Inject hook function
  // 2) Inject InjecteeInfo
  InjecteeInfo injectee_info;

  // 1)
  void* remote_info_memory = VirtualAllocEx(process_handle, NULL, sizeof(injectee_info),
      MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if(remote_info_memory == NULL) {
    return false;
  }
  // 1A)
  int magic_bytes_value = 'HOON';
  byte* magic_bytes = reinterpret_cast<byte*>(&magic_bytes_value);
  byte* replacement_bytes = reinterpret_cast<byte*>(&remote_info_memory);
  ReplaceMagicBytes(clipcursor_hook_bytes, clipcursor_hook_size, magic_bytes, sizeof(void*),
      replacement_bytes);
  // 1B)
  if(!InjectFunctionData(process_handle, clipcursor_hook_bytes, clipcursor_hook_size, 
      reinterpret_cast<void**>(&injectee_info.clipcursor_hook))) {
    return false;
  }
  // 2)
  InitInjecteeFunctionTable(injectee_info.functions);
  InitStringTable(injectee_info.string_table);
  SIZE_T info_bytes_written;
  bool info_success = WriteProcessMemory(process_handle, remote_info_memory, &injectee_info,
    sizeof(injectee_info), &info_bytes_written);
  if(!info_success || info_bytes_written != sizeof(injectee_info)) {
    return false;
  }

  void* remote_proc_memory;
  if(!InjectFunctionData(process_handle, remote_proc_bytes, remote_proc_size, 
      &remote_proc_memory)) {
    return false;
  }

  HANDLE remote_thread = CreateRemoteThread(process_handle, NULL, 0, 
    reinterpret_cast<LPTHREAD_START_ROUTINE>(remote_proc_memory), remote_info_memory, 0, NULL);
  if(remote_thread == NULL) {
    return false;
  }

  if(WaitForSingleObject(remote_thread, 10000) == WAIT_TIMEOUT) {
    return false;
  }

  DWORD exit_code;
  GetExitCodeThread(remote_thread, &exit_code);
  if(exit_code != 0) {
    return false;
  }

  // We can free the remote proc, but the rest of the stuff will just have to stay in memory
  VirtualFreeEx(process_handle, remote_proc_memory, 0, MEM_RELEASE);
  CloseHandle(remote_thread);

  return true;
}

HookInjector::HookInjector(DWORD process_id) 
  : process_id_(process_id) {
}

bool HookInjector::Inject() {
  HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, true, process_id_);
  if(process_handle == NULL) {
    return false;
  }

  size_t remote_proc_size;
  size_t clipcursor_hook_size;

  byte* remote_proc_bytes = 
      GetFunctionBytes(RemoteThreadProc, RemoteThreadProcEnd, remote_proc_size);
  byte* clipcursor_hook_bytes = 
      GetFunctionBytes(ClipCursorHook, ClipCursorHookEnd, clipcursor_hook_size);
  
  bool success = InjectProcAndHook(process_handle, remote_proc_bytes, remote_proc_size,
      clipcursor_hook_bytes, clipcursor_hook_size);

  CloseHandle(process_handle);

  delete remote_proc_bytes;
  delete clipcursor_hook_bytes;

  return success;
}