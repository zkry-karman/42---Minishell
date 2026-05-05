*This project has been created as part of the 42 curriculum by <zkarman>, <kzhu>*

# minishell

## Description 

This project aims to create a simple shell being able to execute various commands that can be seen in a normal shell. These commands include the following:

   - echo
   - cd
   - pwd
   - export
   - unset
   - env
   - exit

Other commands that were also implemented in the scope of this project :

   - Redirections 
      - '<' for redirection input
      - '>' for redirection output
      - '<<' for reading input directly from the terminal
      - '>>' appending output to the end of a given FD
   - Pipes '|'
   - Environmental variables Expansion '$' '$?'
   - Interactive mode commands:
      - 'ctrl-C' for displaying a new prompt on a new line
      - 'ctrl-D' for exiting the shell
      - 'ctrl-\'

In order to divide the work for this project, the work load was split into two parts. The first part created by <kzhu>, and the second by <zkarman>.

Part I. Parsing.

The parsing phase acts as the frontline of the Minishell, it takes a raw string of text from the user, validate it and translate into a structured, organized format that the executor can safely read.
The parsing pipeline is divided into 4 distinct phases:
1. Tokenization
When the user put input via readline, the raw string is sent to the lexer (build_token), which reads the string and splits it into a linked list of tokens while assigning the type individually.
2. Syntax Validation
Before building any commands, the token list has to pass a strtict 'sytntax_checker'. This function acts as a security guard, looking for illegal token combinations (such as '| |' '< >' or ending line with a pipe). If a syntax error is found, the parser will free the tokens, print a error message alike real Bash enviorment.
3. Command Construction
Once the tokens are built and validated, they're passed to (build_cmds), which separates the command arguments from the redirection instructions, linking them together based on pipes. The raw input has been transformed into a clean, iterable list of commands ready for execution.
4. Signal Handling & State Management
The parser also handles asynchronous OS Signals, using signal(), the shell correctly intercepts SIGINT (Ctrl + C) to redraw the prompt without killing the program, ignores SIGQUIT(Ctrl +), and gracefully shuts down frees all memory upon receiving an EOF (Ctrl + D).

Part II. Execution

The execution part of this project was split into 3 parts.
1. Heredocs
Because HEREDOCS take priority within a shell, this is the first thing to be executed when reading the commands. The command list is looped through to see if any of the commands are tokenized as a HEREDOC. If a HEREDOC is found, run it, allowing the user to input lines until the HEREDOC limiter is found. from there, we exit the HEREDOC, and continue reading the other commands. 
2. Reading Commands
The bulk of the execution part happens within the loop 'loop_cmds()'. Here we look at the current command, and decide whether it is a built in command that is managed/executed by zkarman directly, or if it is a system command that needs to be ran by execve. If it's a built in function, the function that needs to be executed is matched with the command, and then executed. Otherwise, the command is piped, and the correct infiles/outfiles are duplicated then closed while working on the child process, and finally the command is found within the system and executed. 
3. Cleanup and exiting
Once the commands are done being read, any allocated memory is freed, and the program is exited proving the necessary exit status based on the success/failure of the commands that were to be executed. 

## Instructions

cd minishell
make
./minishell

## Resources
Let's build a super simple shell in C 
https://www.youtube.com/watch?v=yTR00r8vBH8
Minishell - Architecture (big picture)
https://whimsical.com/minishell-architecture-big-picture-7b9N8PL3qHrddbs977mQ2J

Norminette Compliance Strategy: AI was used to brainstorm architectural refactoring strategies (such as safely splitting the main loop execution pipeline) to adhere to the strict 25-line function limits without breaking the logic flow.
Concept Clarification: AI served as an interactive tutor to clarify low-level C concepts, such as the exact asynchronous execution flow of system signal() interrupts (SIGINT) versus standard terminal EOF (Ctrl+D).