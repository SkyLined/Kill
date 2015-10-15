HRESULT fhGetSnapshot(DWORD dwFlags, DWORD dwProcessId, HANDLE &hSnapshot) {
  HRESULT hResult;
  hSnapshot = CreateToolhelp32Snapshot(dwFlags, dwProcessId);
  if (hSnapshot == INVALID_HANDLE_VALUE) {
    hResult = HRESULT_FROM_WIN32(GetLastError());
  } else {
    hResult = S_OK;
  }
  return hResult;
}
