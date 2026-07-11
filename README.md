# Custom Unix Shell

A Unix shell written in C as part of learning Linux system programming.

## Current Features

- Read commands from standard input
- Tokenize input into command-line arguments
- Execute Linux commands using `fork()`, `execvp()`, and `wait()`

## Build

```bash
gcc shell.c -o shell
```

## Run

```bash
./shell
```

Example:

```
$ ls -l
$ echo Hello
$ sleep 2
```

## Learning Goals

This project is helping me understand:

- Processes
- fork()
- exec()
- wait()
- Memory management in C
- Building a Unix shell from scratch
