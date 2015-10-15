HRESULT fhTerminateAllProcessesForExecutableName(const _TCHAR* sExecutableName) {
  HRESULT hResult;
  DWORD dwProcessId;
  BOOL bProcessFound;
  do {
    hResult = fhGetProcessIdForExecutableName(sExecutableName, dwProcessId, bProcessFound);
    if (!SUCCEEDED(hResult)) return hResult;
    if (bProcessFound) {
      hResult = fhTerminateProcessForId(dwProcessId);
      if (!SUCCEEDED(hResult)) return hResult;
    }
  } while (bProcessFound);
  return hResult;
}
