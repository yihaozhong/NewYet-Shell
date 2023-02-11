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

The academic integrity policy will be strictly enforced. All violations will be reported to the Director of Undergraduate Studies and subject to disciplinary actions. PLEASE DO NOT COPY THIS WORK. You may not copy, reproduce, distribute, publish, display, perform, modify, create derivative works, transmit, or in any way exploit any such content, nor may you distribute any part of this content over any network, including a local area network, sell or offer it for sale, or use such content to construct any kind of database. 
