Kill
==================

A simple command line application to reliably terminate processes.

Usage
-----
There are two ways to use the application. First, you can simply kill a process
for a given process id:

    Kill --pids <pid> [<pid> [...]]

Or you can kill all processes running a given binary:

    Kill <binary name.exe> [<binary name.exe> [...]]

In the later case, Kill will attempt to find all processes that are running the
given binary and get their process ids, before killing the processes using those
process ids similar to the first way of executing Kill.

Kill will attempt to open the process for the given process id in order to kill
it. If that fails in a way that indicate that no process is running with that
id, it assumes that the process was already terminated:

    H:\dev\Kill\bin>Kill_x64.exe --pids 1000 1002
    * Killing process with id 1000
    * Process 9999 does not exist.
    * Killing process with id 1002
    * Process 9999 does not exist.

If a process does not terminate within 1 second of terminating it, Kill will
return an error:

    H:\dev\Kill>Kill_x64.exe --pids 1000
    * Killing process with id 1000
    - Cannot wait for termination of process 1000 (error 00000000).

(This can happen if the process is suspended).

If a process cannot be opened or terminated, Kill will return an error as well:

    H:\dev\Kill>Kill_x64.exe --pids 1000
    * Killing process with id 1000
    - Cannot terminate process 1000 (error 00000005).

If all processes was already terminated, or successfully terminated, kill will
report this:

    H:\dev\Kill\bin>Kill_x64.exe --pids 1000 1002
    * Killing process with id 1000
    * Process 1000 was already terminated.
    * Killing process with id 1002
    + Terminated process 1002.

Kill will terminate as soon as it encounters an error. This means that if you
are attempting to kill multiple processes, Kill may not attempt to terminate
all of them if there is an error.

Errorlevel
----------
If there were any errors Kill will exit with errorlevel 1, otherwise it will
exit with errorlevel 0.

Parsing output
--------------
You may want to parse the output of Kill to extract error messages. All lines
should start with a character that indicates what type of information is on that
line. There are three different characters in use:
* `*` indicates a message containing progress notification.
* `+` indicates a message about a successfully terminated process.
* `-` indicates a fatal error message. 

Download
--------
You can download the most up-to-date pre-build executables, including
.pdb files for debugging, from the [repository]
(https://github.com/SkyLined/Kill/tree/master/bin).

Please note that the 64-bit executable will (obviously) not run on 32-bit
windows builds, and that the 32-bit executable will not work on 64-bit windows
builds.
