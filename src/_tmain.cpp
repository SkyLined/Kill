#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

typedef BOOL (WINAPI *tIsWow64Process)(HANDLE, PBOOL);
tIsWow64Process _IsWow64Process;

#include "fhGetSnapshot.h"
#include "fbCloseHandleAndUpdateResult.h"
#include "fhGetProcessIdForExecutableName.h"
#include "fhProcessExistsForId.h"
#include "fhTerminateProcessForId.h"
#include "fhTerminateAllProcessesForExecutableName.h"
#include "fuKill.h"

UINT _tmain(UINT uArgumentsCount, _TCHAR* asArguments[]) {
  return fuKill(uArgumentsCount, asArguments);
}