Kill
==================

A simple command line application to reliably terminate processes.

Usage
-----
Kill can be used to kill processes by process id or by binary name:

    Kill <pid or binary name> [<pid or binary name> [...]]

If you specify a process id, Kill will attempt to terminate the process for the
given process id. If you specify a binary name, Kill will attempt to find all
processes that are running the given binary, get their process ids and terminate
the processes for these process ids.

Kill will attempt to terminate a process by opening the process using the
process id, getting its exit code to determine if it's still running, and
terminating it if it is. If opening the process fails in a way that indicate
that no such process is running, or if the exit code indicates this, it will
ignore the process and assume it was already terminated:

    H:\dev\Kill\bin>Kill_x64.exe 1000 1002
    * Killing process with id 1000...
    * Process 1000 does not exist.
    * Killing process with id 1002...
    * Process 1002 was already terminated.

If a processes is successfully terminated Kill will report this:

    H:\dev\Kill\bin>Kill_x64.exe --pids 1000
    * Killing process with id 1000...
    + Terminated process 1000.

If a process cannot be opened or terminated, or if a process does not actually
terminate within 1 second of attempting to terminating it, Kill will return an
error:

    H:\dev\Kill>Kill_x64.exe 1000 1002 1004
    * Killing process with id 1000...
    - Cannot open process 1000 (error 00000005).
    * Killing process with id 1002...
    - Cannot terminate process 1002 (error 00000005).
    * Killing process with id 1004...
    - Cannot wait for termination of process 1004 (error 00000000).

Kill will attempt to terminate processes in the order given on the command line.
It will continue to attempt to terminate processes if it encounters an error.

Exit code
---------
If there were any errors Kill will exit with exit code 1, otherwise it will exit
with exit code 0.

Parsing output
--------------
You may want to parse the output of Kill to extract error messages. All lines
should start with a character that indicates what type of information is on that
line. There are three different characters in use:
* `*` indicates a message containing progress notification.
* `+` indicates a message about a successfully terminated process.
* `-` indicates a fatal error message. 

fKillProcessesUntilTheyAreDead.py
---------------------------------
The fKillProcessesUntilTheyAreDead python function can be used to make sure one
or more processes are terminated. You can specify a list of process ids and
process binary names and it will execute Kill up to 60 times, waiting one second
in between executions, until all processes have terminated. If processes cannot
be terminated after 60 attempts, it throws an assertion failure exception.

Download
--------
You can download the most up-to-date pre-build executables, including
.pdb files for debugging, from the [repository]
(https://github.com/SkyLined/Kill/tree/master/bin).

Please note that the 64-bit executable will (obviously) not run on 32-bit
windows builds, and that the 32-bit executable will not work on 64-bit windows
builds.
