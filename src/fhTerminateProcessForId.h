HRESULT fhTerminateProcessForId(DWORD dwProcessId) {
  HRESULT hResult;
  HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION | SYNCHRONIZE, FALSE, dwProcessId);
  if (!hProcess) {
    if (GetLastError() == ERROR_INVALID_PARAMETER) {
      _tprintf(_T("* Process %d does not exist.\r\n"), dwProcessId);
      hResult = S_OK;
    } else {
      _tprintf(_T("- Cannot open process %d (error %08X).\r\n"), dwProcessId, GetLastError());
      hResult = HRESULT_FROM_WIN32(GetLastError());
    }
  } else {
    DWORD dwExitCode;
    if (!GetExitCodeProcess(hProcess, &dwExitCode)) {
      _tprintf(_T("- Cannot get process %d exit code (error %08X).\r\n"), dwProcessId, GetLastError());
      hResult = HRESULT_FROM_WIN32(GetLastError());
    } else if (dwExitCode != STILL_ACTIVE) {
      _tprintf(_T("* Process %d was already terminated.\r\n"), dwProcessId);
      hResult = S_OK;
    } else if (!TerminateProcess(hProcess, 0)) {
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
