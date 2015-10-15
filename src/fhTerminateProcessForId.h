HRESULT fhTerminateProcessForId(DWORD dwProcessId) {
  HRESULT hResult;
  HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, FALSE, dwProcessId);
  if (!hProcess) {
    if (GetLastError() == ERROR_INVALID_PARAMETER) {
      _tprintf(_T("* Process %d no longer exists.\r\n"), dwProcessId);
      hResult = S_OK;
    } else {
      _tprintf(_T("- Cannot open process %d (error %08X).\r\n"), dwProcessId, GetLastError());
      hResult = HRESULT_FROM_WIN32(GetLastError());
    }
  } else {
    if (!TerminateProcess(hProcess, 0)) {
      _tprintf(_T("- Cannot terminate process %d (error %08X).\r\n"), dwProcessId, GetLastError());
      hResult = HRESULT_FROM_WIN32(GetLastError());
    } else if (WaitForSingleObject(hProcess, 1000) != WAIT_OBJECT_0) {
      _tprintf(_T("- Cannot wait for termination of process %d (error %08X).\r\n"), dwProcessId, GetLastError());
      hResult = HRESULT_FROM_WIN32(GetLastError());
    } else {
      _tprintf(_T("+ Terminated process %d.\r\n"), dwProcessId);
      hResult = S_OK;
    }
    if (!fbCloseHandleAndUpdateResult(hProcess, hResult)) {
      _tprintf(_T("- Cannot close process %d (error %08X).\r\n"), dwProcessId, GetLastError());
    }
  }
  return hResult;
}
