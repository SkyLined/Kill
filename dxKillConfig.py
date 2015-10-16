import os;
sBaseDir = os.path.dirname(__file__);

try:
  from dxConfig import dxConfig;
except:
  dxConfig = {};

# Add Kill group if it doesn't exist.
dxKillConfig = dxConfig.setdefault("Kill", {});
# Add default values where no values have been supplied:
for (sName, xValue) in {
  "sKillBinaryPath_x86": os.path.join(sBaseDir, "bin", "Kill_x86.exe"),
  "sKillBinaryPath_x64": os.path.join(sBaseDir, "bin", "Kill_x64.exe"),
}.items():
  if sName not in dxKillConfig:
    dxKillConfig[sName] = xValue;
