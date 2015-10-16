HRESULT fhTerminateAllProcessesForExecutableName(const _TCHAR* sExecutableName) {
  HRESULT hResult;
  DWORD dwProcessId;
  BOOL bProcessFound;
  do {
    hResult = fhGetProcessIdForExecutableName(sExecutableName, dwProcessId, bProcessFound);
    if (!SUCCEEDED(hResult) || !bProcessFound) return hResult;
    hResult = fhTerminateProcessForId(dwProcessId);
  } while (SUCCEEDED(hResult));
  return hResult;
}
