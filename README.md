# NewYet-Shell
New Yet Shell: a main command-line interface between a user and the operating system

This is a command-line interpreter. It works as follows:

1. It prompts you to enter a command.
2. It interprets the command you entered.
3. If you entered a built-in command  (cd, exit, fg, jobs), then the shell runs that command.
4. If you entered an external program (e.g., /bin/ls), or multiple programs connected through pipes (e.g., ls -l | less), then the shell creates child processes, executes these programs, and waits for all these processes to either terminate or be suspended.
5. If you entered something wrong, then the shell prints an error message.
6. Rinse and repeat until you press Ctrl-D to close STDIN or enter the built-in command exit, at which point the shell exits.

<hr>

 - [x] handle signals and I/O redirection
 - [x] Linux programming environment and the shell.
 - [x] processes created, destroyed, and managed.
 - [x] OS and system calls.


