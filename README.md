# Minishell

*This project has been created as part of the 42 curriculum by ekamar, azorlutu.*

---

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Architecture Overview](#architecture-overview)
  - [Data Structures](#data-structures)
  - [Module Breakdown](#module-breakdown)
- [Execution Flow](#execution-flow)
- [Instructions](#instructions)
  - [Requirements](#requirements)
  - [Compilation](#compilation)
  - [Usage](#usage)
  - [Cleaning](#cleaning)
- [Built-in Commands](#built-in-commands)
- [Signal Handling](#signal-handling)
- [Redirections and Pipes](#redirections-and-pipes)
- [Environment Variables](#environment-variables)
- [Quote Handling](#quote-handling)
- [Edge Cases](#edge-cases)
- [Project Structure](#project-structure)
- [Resources](#resources)
- [AI Usage Disclosure](#ai-usage-disclosure)

---

## Description

**Minishell** is a simplified shell implementation written in C, inspired by **bash**. This project is part of the 42 school curriculum and aims to provide students with a deep understanding of how Unix shells work internally.

The shell reads commands from the user, parses them, expands environment variables, handles redirections and pipes, and executes the commands. It supports both built-in commands (like `cd`, `echo`, `export`) and external programs found in the system's `PATH`.

### Goal

The primary goal of this project is to:
- Understand how a command-line interpreter works
- Learn process creation and management using `fork()`, `execve()`, `wait()`, and `pipe()`
- Handle signals (`SIGINT`, `SIGQUIT`) properly
- Manage file descriptors for redirections
- Implement a lexer/tokenizer and parser for command-line input

---

## Features

| Feature | Description |
|---------|-------------|
| **Command History** | Remembers previously entered commands using `readline` library |
| **Executable Search** | Searches for executables in `PATH` or uses absolute/relative paths |
| **Single Quote Handling** | Prevents interpretation of meta-characters inside `'...'` |
| **Double Quote Handling** | Prevents interpretation of meta-characters except `$` inside `"..."` |
| **Environment Variable Expansion** | Expands `$VAR` and `$?` (last exit status) |
| **Input Redirection (`<`)** | Redirects input from a file |
| **Output Redirection (`>`)** | Redirects output to a file (overwrites) |
| **Append Redirection (`>>`)** | Redirects output to a file (appends) |
| **Here Document (`<<`)** | Reads input until a delimiter is encountered |
| **Pipes (`\|`)** | Connects stdout of one command to stdin of another |
| **Signal Handling** | Proper handling of `Ctrl+C`, `Ctrl+D`, `Ctrl+\` |
| **Built-in Commands** | `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |

---

## Architecture Overview

### Data Structures

The project uses several key data structures defined in `includes/minishell.h`:

#### `t_token` - Token Node
```c
typedef struct s_token
{
    t_token_type    type;       // TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIRECT_*, etc.
    t_quote_type    quote;      // NO_QUOTE, SINGLE_QUOTE, DOUBLE_QUOTE
    char            *value;     // The actual string value
    struct s_token  *next;      // Linked list pointer
}   t_token;
```

#### `t_command` - Command Node
```c
typedef struct s_command
{
    char                **args;                    // Command arguments array
    t_redirection       *redirects;                // Linked list of redirections
    struct s_command    *next_pipe;                // Next command in pipeline
    int                 is_quoted_empty_command;   // Handle "" or '' edge case
}   t_command;
```

#### `t_redirection` - Redirection Node
```c
typedef struct s_redirection
{
    t_token_type            type;                // Redirection type
    char                    *filename;           // Target filename
    t_quote_type            redir_quote;         // Quote type for heredoc
    char                    *delimiter_raw;      // Raw heredoc delimiter
    char                    *delimiter_expanded; // Expanded heredoc delimiter
    int                     fd;                  // File descriptor
    struct s_redirection    *next;               // Next redirection
}   t_redirection;
```

#### `t_env` - Environment Variable Node
```c
typedef struct s_env
{
    char            *key;    // Variable name
    char            *value;  // Variable value
    struct s_env    *next;   // Linked list pointer
}   t_env;
```

#### `t_minishell` - Main Shell State
```c
typedef struct s_minishell
{
    char        *input;          // Raw input from readline
    t_env       *env_list;       // Environment variables linked list
    t_token     *tokens;         // Tokenized input
    t_command   *command_list;   // Parsed commands
    char        **env_array;     // Env as array for execve
    int         last_exit_code;  // $? value
    int         has_syntax_error;// Syntax error flag
    pid_t       *temp_pids;      // Child process IDs
}   t_minishell;
```

### Module Breakdown

The source code is organized into 8 main modules:

| Module | Files | Purpose |
|--------|-------|---------|
| **main** | `main.c`, `minishell.c` | Entry point, shell loop, initialization |
| **tokenizer** | 6 files | Lexical analysis, converts input to tokens |
| **parser** | 3 files | Syntactic analysis, converts tokens to command structures |
| **expander** | 3 files | Environment variable expansion (`$VAR`, `$?`) |
| **executor** | 11 files | Command execution, pipes, redirections, heredocs |
| **builtins** | 11 files | Built-in command implementations |
| **env** | 5 files | Environment variable management |
| **utils** | 3 files | Signal handlers, memory cleanup utilities |

---

## Execution Flow

The shell follows a **Read-Eval-Print Loop (REPL)** pattern:

```
┌─────────────────────────────────────────────────────────────────────────┐
│                              SHELL LOOP                                  │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                          │
│   1. READLINE        ──►  Read user input with prompt "minishell> "     │
│         │                                                                │
│         ▼                                                                │
│   2. TOKENIZER       ──►  Convert input string to token linked list     │
│         │                  (TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIRECT_*)   │
│         ▼                                                                │
│   3. EXPANDER        ──►  Expand $VAR and $? in tokens                  │
│         │                                                                │
│         ▼                                                                │
│   4. PARSER          ──►  Convert tokens to command structure           │
│         │                  (args, redirections, pipes)                  │
│         ▼                                                                │
│   5. EXECUTOR        ──►  Execute commands                              │
│         │                  - Set up pipes between commands              │
│         │                  - Handle redirections (<, >, >>, <<)         │
│         │                  - Fork child processes                       │
│         │                  - Run built-ins or external programs         │
│         ▼                                                                │
│   6. CLEANUP         ──►  Free memory, wait for children                │
│         │                                                                │
│         └────────────────► Loop back to step 1                          │
│                                                                          │
└─────────────────────────────────────────────────────────────────────────┘
```

### Detailed Flow

1. **Input Reading**: `readline("minishell> ")` reads user input with line editing support
2. **History**: `add_history()` saves command for arrow-key navigation
3. **Tokenization**: Input is split into tokens (words, operators, quotes preserved)
4. **Expansion**: Environment variables are replaced with their values
5. **Parsing**: Tokens are organized into command structures with redirections
6. **Execution**: Commands are executed (fork for external, direct for builtins)
7. **Cleanup**: Memory is freed, file descriptors are closed

---

## Instructions

### Requirements

- **Operating System**: Unix-like (Linux, macOS)
- **Compiler**: `cc` (clang or gcc)
- **Libraries**: 
  - `readline` library (for command line editing)
  - `libft` (included in project)

**Installing readline on Debian/Ubuntu:**
```bash
sudo apt-get install libreadline-dev
```

**Installing readline on macOS:**
```bash
brew install readline
```

### Compilation

Clone the repository and compile:

```bash
git clone https://github.com/ahmettzorlutuna/minishell.git minishell
cd minishell
make
```

The `make` command will:
1. Compile the `libft` library
2. Compile all source files with flags `-Wall -Wextra -Werror`
3. Link with the `readline` library
4. Produce the `minishell` executable

### Usage

Run the shell:

```bash
./minishell
```

You will see the prompt:
```
minishell>
```

**Example session:**
```bash
minishell> echo "Hello, World!"
Hello, World!
minishell> ls -la | grep minishell
-rwxr-xr-x  1 user  staff  123456 Jan 19 12:00 minishell
minishell> export MY_VAR="42 Istanbul"
minishell> echo $MY_VAR
42 Istanbul
minishell> cat << EOF
heredoc> This is a
heredoc> here document
heredoc> EOF
This is a
here document
minishell> exit
```

### Cleaning

```bash
make clean    # Remove object files
make fclean   # Remove object files and executable
make re       # Rebuild everything
```

**Valgrind check (memory leaks):**
```bash
make v
# or
valgrind --show-leak-kinds=all --leak-check=full --suppressions=readline.supp ./minishell
```

---

## Built-in Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| `echo` | `echo [-n] [text...]` | Prints text to stdout. `-n` suppresses trailing newline |
| `cd` | `cd [path]` | Changes current directory. `cd -` returns to previous directory |
| `pwd` | `pwd` | Prints current working directory |
| `export` | `export [VAR=value]` | Sets environment variable. Without args, lists all exported variables sorted |
| `unset` | `unset VAR` | Removes environment variable |
| `env` | `env` | Lists all environment variables |
| `exit` | `exit [code]` | Exits shell with optional exit code (modulo 256) |

### Built-in Implementation Details

- **`cd`**: Updates `PWD` and `OLDPWD` environment variables
- **`export`**: Validates variable names (must start with letter or `_`, contain only alphanumerics and `_`)
- **`exit`**: Handles numeric validation and "too many arguments" error

---

## Signal Handling

The shell handles signals differently based on context:

| Signal | Interactive Mode | During Command Execution | Heredoc |
|--------|------------------|--------------------------|---------|
| `Ctrl+C` (SIGINT) | New prompt on new line | Terminates current command | Exits heredoc, returns to prompt |
| `Ctrl+D` (EOF) | Exits shell | Sends EOF to running command | Exits heredoc |
| `Ctrl+\` (SIGQUIT) | Ignored | Terminates with core dump | Ignored |

### Implementation

A **single global variable** `g_signal_flag` is used to store the received signal number:

```c
extern int g_signal_flag;
```

This complies with the 42 norm that allows only one global variable for signal handling.

### Exit Codes

- Normal exit: Exit code of last command
- `Ctrl+C`: Exit code 130 (128 + 2)
- `Ctrl+\`: Exit code 131 (128 + 3)

---

## Redirections and Pipes

### Redirections

| Operator | Name | Description |
|----------|------|-------------|
| `<` | Input Redirection | Read input from file instead of stdin |
| `>` | Output Redirection | Write output to file (overwrite) |
| `>>` | Append Redirection | Write output to file (append) |
| `<<` | Here Document | Read input until delimiter line |

**Examples:**
```bash
# Input redirection
cat < input.txt

# Output redirection (creates/overwrites file)
echo "Hello" > output.txt

# Append redirection (creates/appends to file)
echo "World" >> output.txt

# Here document
cat << DELIMITER
This text will be
passed to cat
DELIMITER
```

### Pipes

Pipes connect the stdout of one command to the stdin of the next:

```bash
# Simple pipe
ls -la | grep ".c"

# Multiple pipes
cat file.txt | grep "pattern" | wc -l
```

**Implementation:**
- Uses `pipe()` system call to create pipe file descriptors
- `fork()` creates child processes for each command
- `dup2()` redirects stdin/stdout to pipe ends
- Parent process waits for all children with `waitpid()`

---

## Environment Variables

### Expansion Rules

| Syntax | Description |
|--------|-------------|
| `$VAR` | Expands to value of VAR |
| `$?` | Expands to exit status of last command |
| `"$VAR"` | Expands inside double quotes |
| `'$VAR'` | NOT expanded (literal string) |

**Examples:**
```bash
minishell> export NAME="John"
minishell> echo Hello $NAME
Hello John
minishell> echo 'Hello $NAME'
Hello $NAME
minishell> echo "Hello $NAME"
Hello John
minishell> false
minishell> echo $?
1
```

### Environment Management

- Environment is stored as a **linked list** (`t_env`)
- Also converted to **array format** (`char **env_array`) for `execve()`
- Functions: `get_env_value()`, `set_env_value()`, `unset_env_value()`

---

## Quote Handling

### Single Quotes (`'...'`)

- **All characters** are treated literally
- No variable expansion
- No escape sequences

### Double Quotes (`"..."`)

- **Most characters** are treated literally
- `$` triggers variable expansion
- Metacharacters like `|`, `<`, `>` are not interpreted

### Edge Cases

```bash
# Mixed quotes
echo "Hello"'World'    # Output: HelloWorld

# Empty quotes
echo ""                # Output: (empty line)

# Quotes in middle of word
echo hel"lo wor"ld     # Output: hello world
```

---

## Edge Cases

The shell handles many edge cases:

1. **Unclosed Quotes**: Returns syntax error
2. **Empty Commands**: Properly handled with no error
3. **Quoted Empty Commands**: `""` or `''` as first argument shows error
4. **cd with Pipes**: `cd | echo hi` - cd doesn't work in child process
5. **Variable Names Starting with Number**: `export 1VAR=x` shows error
6. **Exit with Non-numeric Argument**: Shows "numeric argument required"
7. **Exit Modulo 256**: `exit 256` exits with code 0
8. **Multiple Redirections**: Last redirection of same type takes effect
9. **Signals in Heredoc**: Ctrl+C properly exits heredoc

---

## Project Structure

```
minishell/
├── Makefile                 # Build configuration
├── README.md               # This file
├── includes/
│   └── minishell.h         # Header file with all declarations
├── libft/                  # Custom C library (42 project)
│   └── ...
└── srcs/
    ├── main/
    │   ├── main.c          # Entry point, shell loop
    │   └── minishell.c     # Initialization functions
    ├── tokenizer/
    │   ├── tokenizer.c     # Main tokenization logic
    │   ├── token_list.c    # Token linked list operations
    │   ├── token_utils.c   # Operator and whitespace handling
    │   ├── token_utils2.c  # Quote length measurement
    │   ├── get_combined_token.c  # Combined token extraction
    │   └── free_tokenizer.c      # Token memory cleanup
    ├── parser/
    │   ├── parser.c        # Main parsing logic
    │   ├── parser_utils.c  # Helper functions
    │   └── parser_node_utils.c  # Command node operations
    ├── expander/
    │   ├── expander.c      # Main expansion logic
    │   ├── expander_utils.c     # Variable extraction
    │   └── expander_utils2.c    # Exit status expansion
    ├── executor/
    │   ├── executor.c      # Pipeline execution
    │   ├── executor_utils.c     # Path resolution
    │   ├── executor_utils2.c    # Process management
    │   ├── executor_utils3.c    # Builtin execution
    │   ├── executor_utils4.c    # Pipeline forking
    │   ├── executor_utils5.c    # Pipe count utilities
    │   ├── handle_null_commands.c  # Empty command handling
    │   ├── heredoc.c       # Here document handling
    │   ├── heredoc_loop.c  # Heredoc read loop
    │   ├── heredoc_utils.c # Heredoc helpers
    │   └── redirection.c   # File redirection
    ├── builtins/
    │   ├── cd/
    │   │   ├── builtin_cd.c
    │   │   └── builtin_cd_utils.c
    │   ├── echo/
    │   │   └── builtin_echo.c
    │   ├── env/
    │   │   └── builtin_env.c
    │   ├── exit/
    │   │   ├── builtin_exit.c
    │   │   └── builtin_exit_utils.c
    │   ├── export/
    │   │   ├── builtin_export.c
    │   │   └── builtin_export_utils.c
    │   ├── pwd/
    │   │   └── builtin_pwd.c
    │   ├── unset/
    │   │   └── builtin_unset.c
    │   └── utils/
    │       └── builtin_utils.c
    ├── env/
    │   ├── env_array.c     # env to array conversion
    │   ├── env_list.c      # Linked list operations
    │   ├── env_list_utils.c
    │   ├── free_env.c      # Memory cleanup
    │   └── sort_env_list.c # Sorting for export display
    └── utils/
        ├── handle_signals.c     # Signal setup functions
        ├── signal_handlers.c    # Signal handler implementations
        └── free_loop.c          # Per-iteration cleanup
```

---

## Resources

### Official Documentation

- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/bash.html) - Official bash documentation
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) - POSIX standard for shells
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html) - Documentation for readline

### System Calls and Functions

- **Process Control**: `fork(2)`, `execve(2)`, `wait(2)`, `waitpid(2)`, `exit(3)`
- **File Descriptors**: `open(2)`, `close(2)`, `read(2)`, `write(2)`, `dup(2)`, `dup2(2)`
- **Pipes**: `pipe(2)`
- **Signals**: `signal(2)`, `sigaction(2)`, `kill(2)`
- **Directory**: `getcwd(3)`, `chdir(2)`, `opendir(3)`, `readdir(3)`, `closedir(3)`
- **Environment**: `getenv(3)`

### Tutorials and Articles

- [Writing a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/) - Excellent tutorial by Stephen Brennan
- [Linux Shell Scripting Tutorial](https://bash.cyberciti.biz/guide/Main_Page) - Understanding shell behavior
- [The TTY Demystified](http://www.linusakesson.net/programming/tty/) - Understanding terminal I/O

### 42 Specific Resources

- [42 Minishell Subject](https://projects.intra.42.fr/projects/minishell) - Official project subject
- [Harm-Smits 42 Docs](https://harm-smits.github.io/42docs/) - Unofficial 42 documentation

---

## AI Usage Disclosure

AI tools were used in the following aspects of this project:

### README Documentation
- **Claude (Anthropic)** was used to generate this comprehensive README.md file
- The AI analyzed the project structure, source code, and header files to create accurate documentation
- All technical information was verified against the actual codebase

### Code Development
- The minishell implementation was developed by the student(s) without AI assistance in coding
- AI was not used for writing, debugging, or optimizing the shell source code

### Note
According to 42's academic integrity policies, this disclosure is provided to maintain transparency about AI usage in the project documentation process.

---

## Authors

- **ekamar** - 42 Istanbul
- **azorlutu** - 42 Istanbul

---

## License

This project is part of the 42 school curriculum. See the [LICENSE](LICENSE) file for details.