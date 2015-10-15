BOOL fbCloseHandleAndUpdateResult(HANDLE hHandle, HRESULT &hResult) {
  // Close the handle, if this fails and the hResult is not an error, update hResult.
  // Return TRUE if the handle was successfully closed.
  if (!CloseHandle(hHandle)) {
    if (SUCCEEDED(hResult)) {
      hResult = HRESULT_FROM_WIN32(GetLastError());
    }
    return FALSE;
  }
  return TRUE;
}
