# Tiny Unix Shell (C)

A minimal Unix shell written in **C** to explore how Unix shells work internally by implementing core operating system concepts such as process creation, program execution, synchronization, memory management, and POSIX system calls.

This project is being built incrementally from scratch as a learning exercise, with each feature added while closely following how a real Unix shell behaves.

---

## Features

### Interactive Shell

- Displays the current working directory in the shell prompt.
- Continuously accepts user commands until EOF (`Ctrl+D`).

Example:

```text
/home/user $ ls
/home/user $ pwd
/home/user $ cd projects
/home/user/projects $
```

---

### Command Parsing

Implemented a custom tokenizer without using `strtok()`.

Features:

- Reads commands using `fgets()`
- Handles multiple spaces
- Handles tabs
- Handles newline characters
- Ignores empty commands
- Dynamically allocates memory for each token
- Produces an argument vector compatible with `execvp()`

---

### Process Execution

The shell follows the standard Unix execution model:

1. Read user input
2. Tokenize command
3. Create a child process using `fork()`
4. Execute the requested program using `execvp()`
5. Parent waits using `wait()`
6. Display child's exit status
7. Free allocated memory
8. Display the next prompt

---

### Built-in Commands

#### `cd`

Implemented using `chdir()` inside the shell process.

Unlike external commands, `cd` must execute in the parent shell because changing the working directory of a child process does not affect the shell itself.

Supported:

```text
cd folder
cd ..
```

Invalid arguments are detected and reported.

---

### Exit Status Reporting

The shell reports the exit code returned by child processes.

Uses:

- `wait()`
- `WIFEXITED()`
- `WEXITSTATUS()`

Example:

```text
$ nosuchcommand
execvp: No such file or directory
EXITSTATUS: 1
```

---

### Memory Management

Dynamic memory is allocated for:

- Token array
- Individual command tokens

Memory is released after every command.

Special care is taken to correctly clean up memory during:

- Allocation failures
- Empty input
- Built-in command execution
- Early returns

---

## Error Handling

The shell handles failures from the following system calls:

- `malloc()`
- `fork()`
- `execvp()`
- `wait()`
- `getcwd()`
- `chdir()`

Errors are reported using `perror()` whenever appropriate.

---

## Concepts Explored

- Linux process model
- Process creation using `fork()`
- Program replacement using `execvp()`
- Parent-child synchronization
- Child exit codes
- Built-in shell commands
- Dynamic memory allocation
- Heap vs stack memory
- Command-line argument vectors (`argv`)
- PATH-based executable lookup
- Error handling using `errno`
- String manipulation in C

---

## Project Structure

```text
.
├── .gitignore
├── README.md
└── shell.c

Current implementation includes:
```

Current implementation includes:

- Interactive shell prompt
- Custom tokenizer
- Built-in `cd`
- Process creation
- Command execution
- Exit status reporting
- Memory cleanup

---

## Example Session

```text
/home/user $ pwd
/home/user
EXITSTATUS: 0

/home/user $ ls
Documents
Downloads
myshell.c
EXITSTATUS: 0

/home/user $ cd Documents

/home/user/Documents $ pwd
/home/user/Documents
EXITSTATUS: 0

/home/user/Documents $ invalidcommand
execvp: No such file or directory
EXITSTATUS: 1
```

---

## Building

Compile using GCC:

```bash
gcc shell.c -o shell
```

Run:

```bash
./shell
```

---

## Technologies

- C
- GCC
- Linux
- POSIX APIs

---

## References

- Operating Systems: Three Easy Pieces (OSTEP)
- Linux Manual Pages (`man fork`, `man execvp`, `man wait`, `man chdir`, `man getcwd`)
- POSIX System Call Documentation