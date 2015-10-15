#define fuFromString(sInput) ((UINT)_tcstoul(sInput, NULL, 10))

UINT fuKill(UINT uArgumentsCount, _TCHAR* asArguments[]) {
  HRESULT hResult;
  _IsWow64Process = (tIsWow64Process)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");
  
  BOOL bIsWow64 = FALSE;
  if (_IsWow64Process && !_IsWow64Process(GetCurrentProcess(), &bIsWow64)) {
    _tprintf(_T("- Cannot determine if this is a 64-bit version of Windows (error %08X).\r\n"), GetLastError());
    hResult = HRESULT_FROM_WIN32(GetLastError());
  } else if (bIsWow64) {
    _tprintf(_T("- The 32-bit build of EdgeDbg does not work on a 64-bit build of Windows.\r\n"));
    hResult = E_NOTIMPL;
  } else if (uArgumentsCount < 2) {
    _tprintf(_T("Usage:\r\n"));
    _tprintf(_T("  Kill binary_name.exe [another_binary_name.exe [...]]\r\n"));
    _tprintf(_T("  Kill --pids process_id [another_process_id [...]]\r\n"));
    _tprintf(_T("See documentation at https://github.com/SkyLined/EdgeDbg/ for more details.\r\n"));
    hResult = E_INVALIDARG;
  } else if (uArgumentsCount > 2 && _tcsicmp(asArguments[1], _T("--pids")) == 0) {
    for (UINT uArgumentIndex = 2; uArgumentIndex < uArgumentsCount; uArgumentIndex++) {
      UINT uProcessId = fuFromString(asArguments[uArgumentIndex]);
      _tprintf(_T("* Killing process with id %d...\r\n"), uProcessId);
      hResult = fhTerminateProcessForId(uProcessId);
      if (!SUCCEEDED(hResult)) break;
    }
  } else {
    for (UINT uArgumentIndex = 1; uArgumentIndex < uArgumentsCount; uArgumentIndex++) {
      _tprintf(_T("* Killing processes for binary %s...\r\n"), asArguments[uArgumentIndex]);
      hResult = fhTerminateAllProcessesForExecutableName(asArguments[uArgumentIndex]);
      if (!SUCCEEDED(hResult)) break;
    }
  }
   return SUCCEEDED(hResult) ? 0 : 1;
}
