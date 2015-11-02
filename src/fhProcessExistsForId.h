#include <Psapi.h> // EnumProcesses

HRESULT fhProcessExistsForId(DWORD dwProcessId, BOOL &bExists) {
  bExists = FALSE;
  UINT uAllocatedDWORDS = 0x1000;
  DWORD* adwProcessIds = new DWORD[uAllocatedDWORDS];
  DWORD dwProcessIdsArraySize = 0;
  // First find out how many process ids there are, then allocate that number of DWORDs to store them, then try to
  // get them again and allocate more if the number increased until it finally succeeds...
  while (1) {
    if (!EnumProcesses(adwProcessIds, uAllocatedDWORDS * sizeof(DWORD), &dwProcessIdsArraySize)) {
      _tprintf(_T("- Cannot enumerate process ids (error %08X)...\r\n"), GetLastError());
      return HRESULT_FROM_WIN32(GetLastError());
    };
    if ((UINT)dwProcessIdsArraySize < uAllocatedDWORDS * sizeof(DWORD)) {
      break;
    };
    delete adwProcessIds;
    uAllocatedDWORDS *= 2;
    adwProcessIds = new DWORD[uAllocatedDWORDS];
  };
  for (UINT uIndex = 0; uIndex * sizeof(DWORD) < dwProcessIdsArraySize; uIndex++) {
    if (adwProcessIds[uIndex] == dwProcessId) {
      bExists = TRUE;
    };
  };
  return S_OK;
};