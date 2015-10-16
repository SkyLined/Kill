#define fuFromString(sInput) ((UINT)_tcstoul(sInput, NULL, 10))

UINT fuKill(UINT uArgumentsCount, _TCHAR* asArguments[]) {
  HRESULT hResult;
  _IsWow64Process = (tIsWow64Process)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");
  
  BOOL bIsWow64 = FALSE;
  BOOL bSuccess = TRUE;
  if (_IsWow64Process && !_IsWow64Process(GetCurrentProcess(), &bIsWow64)) {
    _tprintf(_T("- Cannot determine if this is a 64-bit version of Windows (error %08X).\r\n"), GetLastError());
    bSuccess = FALSE;
  } else if (bIsWow64) {
    _tprintf(_T("- The 32-bit build of EdgeDbg does not work on a 64-bit build of Windows.\r\n"));
    bSuccess = FALSE;
  } else if (uArgumentsCount < 2) {
    _tprintf(_T("Usage:\r\n"));
    _tprintf(_T("  Kill target [another_target [...]]\r\n"));
    _tprintf(_T("Where target is either a process id or binary file name.\r\n"));
    _tprintf(_T("See documentation at https://github.com/SkyLined/EdgeDbg/ for more details.\r\n"));
    bSuccess = FALSE;
  } else {
    for (UINT uArgumentIndex = 1; uArgumentIndex < uArgumentsCount; uArgumentIndex++) {
      UINT uProcessId = fuFromString(asArguments[uArgumentIndex]);
      if (uProcessId == 0 || uProcessId == ULONG_MAX) {
        _tprintf(_T("* Killing processes for binary %s...\r\n"), asArguments[uArgumentIndex]);
        hResult = fhTerminateAllProcessesForExecutableName(asArguments[uArgumentIndex]);
      } else {
        _tprintf(_T("* Killing process with id %d...\r\n"), uProcessId);
        hResult = fhTerminateProcessForId(uProcessId);
      }
      bSuccess &= SUCCEEDED(hResult);
    }
  }
   return bSuccess ? 1 : 0;
}
