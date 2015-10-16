import os, re, subprocess, time;
from dxKillConfig import dxKillConfig;
sOSISA = {"AMD64": "x64", "x86": "x86"}[os.getenv("PROCESSOR_ARCHITEW6432") or os.getenv("PROCESSOR_ARCHITECTURE")];

def fKillProcessesUntilTheyAreDead(auProcessIds):
  # Calling os.kill for a running process should kill it without throwing an exception. Calling os.kill for a
  # terminated process should throw an exception with code 87 (invalid argument). So repeatedly calling os.kill until
  # the right exception is thrown can be used to make sure the process is terminated.
  for x in xrange(60):
    assert auProcessIds, "Nothing to kill";
    sKillBinaryPath = dxKillConfig["sKillBinaryPath_%s" % sOSISA];
    assert sKillBinaryPath and os.path.isfile(sKillBinaryPath), \
        "No %s Kill binary found at %s" % (sOSISA, sKillBinaryPath);
    asKillCommand = [sKillBinaryPath] + [str(uProcessId) for uProcessId in auProcessIds];
    oKillProcess = subprocess.Popen(asKillCommand, stdout=subprocess.PIPE, stderr=subprocess.PIPE);
    (sStdOut, sStdErr) = oKillProcess.communicate();
    oKillProcess.stdout.close();
    oKillProcess.stderr.close();
    oKillProcess.wait();
    assert not sStdErr, "Error killing processes:\r\n%s" % sStdErr;
    asErrors = [];
    for sLine in sStdOut.split("\n"):
      oTerminatedOrErrorMatch = re.match(r"^(?:%s)\r?$" % "|".join([
        r"\+ Terminated process (\d+)\.",
        r"\* Process (\d+) does not exist\.",
        r"\* Process (\d+) was already terminated\.",
        r"\- (.*)",
      ]), sLine);
      if oTerminatedOrErrorMatch:
        sProcessId1, sProcessId2, sProcessId3, sError = oTerminatedOrErrorMatch.groups();
        if sError:
          asErrors.append(sError);
        else:
          auProcessIds.remove(long(sProcessId1 or sProcessId2 or sProcessId3));
    if not asErrors:
      return;
    time.sleep(1);
  raise AssertionError("Error killing processes:\r\n%s" % sStdOut);
