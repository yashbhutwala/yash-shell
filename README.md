* * *

# Yash's Shell

This is a basic, lightweight implementation of a UNIX shell.

To compile and run:

```bash
>> make clean
>> make
>> ./bin/yashShell
```

In addition to typical shell commands, features include:

-   When the user presses the “enter” key twice in a row, the shell displays the list of files in the current working directory.

-   `>> touch hello.c; touch hello2.c; rm -rf hello.c`: Reads multiple commands separated by a semicolon character.

-   `>> history` : Prints the previous HIST_LEN number of commands.

-   `>> time` : Prints the current date and time in an human readable form.

-   `>> cd` : Changes the directory.

-   `>> ps -ef | grep root`: Piping.

-   `>> exit` : Exits the yashShell.
