HRESULT fhGetProcessIdForExecutableName(const _TCHAR* sExecutableName, DWORD &dwProcessId, BOOL &bProcessFound) {
  bProcessFound = FALSE;
  HANDLE hProcessesSnapshot;
  HRESULT hResult = fhGetSnapshot(TH32CS_SNAPPROCESS, 0, hProcessesSnapshot);
  if (!SUCCEEDED(hResult)) {
    _tprintf(_T("- Cannot create processes snapshot (HERSULT %08X, error %08X).\r\n"), hResult, GetLastError());
    return hResult;
  }
  PROCESSENTRY32 oProcessEntry32;
  oProcessEntry32.dwSize = sizeof(oProcessEntry32);
  if (!Process32First(hProcessesSnapshot, &oProcessEntry32)) {
    _tprintf(_T("- Cannot get first process from snapshot (error %08X).\r\n"), GetLastError());
    hResult = HRESULT_FROM_WIN32(GetLastError());
  } else do {
    // Get a module snapshot of the process. This may fail, as access may be denied. This is ignored.
    HANDLE hModulesSnapshot;
    HRESULT hSnapshotResult = fhGetSnapshot(TH32CS_SNAPMODULE, oProcessEntry32.th32ProcessID, hModulesSnapshot);
    if (SUCCEEDED(hSnapshotResult)) {
      // We seem to have access to the module list, check if we can get the first module.
      MODULEENTRY32 oModuleEntry32;
      oModuleEntry32.dwSize = sizeof(oModuleEntry32);
      if (!Module32First(hModulesSnapshot, &oModuleEntry32)) {
        if (GetLastError() == ERROR_NO_MORE_FILES) {
          // No: there may be no modules or module information is not available. This is ignored.
        } else {
          _tprintf(_T("- Cannot get first module from snapshot (error %08X).\r\n"), GetLastError());
          hResult = HRESULT_FROM_WIN32(GetLastError());
        }
      }
      // If we can access the first module, scan the moodule list for the requested executable name.
      if (SUCCEEDED(hResult)) do {
        if (_tcsicmp(oModuleEntry32.szModule, sExecutableName) == 0) {
          dwProcessId = oModuleEntry32.th32ProcessID;
          bProcessFound = TRUE;
        }
      } while (SUCCEEDED(hResult) && !bProcessFound && Module32Next(hModulesSnapshot, &oModuleEntry32));
      if (!fbCloseHandleAndUpdateResult(hModulesSnapshot, hResult)) {
        _tprintf(_T("- Cannot close modules snapshot (error %08X).\r\n"), GetLastError());
      }
    }
  } while (SUCCEEDED(hResult) && !bProcessFound && Process32Next(hProcessesSnapshot, &oProcessEntry32));
  if (!fbCloseHandleAndUpdateResult(hProcessesSnapshot, hResult)) {
    _tprintf(_T("- Cannot close processes snapshot (error %08X).\r\n"), GetLastError());
  }
  return hResult;
}
