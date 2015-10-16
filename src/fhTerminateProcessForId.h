HRESULT fhTerminateProcessForId(DWORD dwProcessId) {
  HRESULT hResult;
  BOOL bExists;
  HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION | SYNCHRONIZE, FALSE, dwProcessId);
  if (!hProcess) {
    DWORD dwOpenProcessError = GetLastError();
    hResult = fhProcessExistsForId(dwProcessId, bExists);
    if (SUCCEEDED(hResult)) {
      if (!bExists) {
        _tprintf(_T("* Process %d does not exist.\r\n"), dwProcessId);
      } else {
        _tprintf(_T("- Cannot open process %d (error %08X).\r\n"), dwProcessId, dwOpenProcessError);
        hResult = HRESULT_FROM_WIN32(dwOpenProcessError);
      }
    }
  } else {
    DWORD dwExitCode;
    if (!GetExitCodeProcess(hProcess, &dwExitCode)) {
      _tprintf(_T("- Cannot get process %d exit code (error %08X).\r\n"), dwProcessId, GetLastError());
      hResult = HRESULT_FROM_WIN32(GetLastError());
    } else if (dwExitCode == STILL_ACTIVE && !TerminateProcess(hProcess, 0)) {
      _tprintf(_T("- Cannot terminate process %d (error %08X).\r\n"), dwProcessId, GetLastError());
      hResult = HRESULT_FROM_WIN32(GetLastError());
    } else if (WaitForSingleObject(hProcess, 1000) != WAIT_OBJECT_0) {
      _tprintf(_T("- Cannot wait for termination of process %d (error %08X).\r\n"), dwProcessId, GetLastError());
      hResult = HRESULT_FROM_WIN32(GetLastError());
    } else {
      hResult = S_OK;
    }
    if (!fbCloseHandleAndUpdateResult(hProcess, hResult)) {
      _tprintf(_T("- Cannot close process %d (error %08X).\r\n"), dwProcessId, GetLastError());
    }
    if (SUCCEEDED(hResult)) {
      hResult = fhProcessExistsForId(dwProcessId, bExists);
      if (SUCCEEDED(hResult)) {
        if (bExists) {
          _tprintf(_T("- Process %d is supposed to have terminated but still exists.\r\n"), dwProcessId);
          hResult = ERROR_SIGNAL_REFUSED;
        } else {
          _tprintf(_T("+ Terminated process %d.\r\n"), dwProcessId);
        }
      }
    }
  }
  return hResult;
}
