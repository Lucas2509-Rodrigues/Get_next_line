<h1 align="center">get_next_line</h1>

<p align="center">
	<em>Reading a line from a file descriptor is way too tedious</em>
</p>

<p align="center">
	<img alt="Language" src="https://img.shields.io/badge/language-C-00babc?style=flat-square">
	<img alt="Norminette" src="https://img.shields.io/badge/norminette-passing-2ea44f?style=flat-square">
	<img alt="Part" src="https://img.shields.io/badge/part-mandatory-blue?style=flat-square">
</p>

*This project has been created as part of the 42 curriculum by lurodri2.*

---

## 📖 Description

**get_next_line** is a C function that returns, one call at a time, the next line of a file descriptor. Calling it in a loop reads a whole file (or any other stream, such as the standard input) line by line, without ever loading it entirely into memory.

```c
char	*get_next_line(int fd);
```

| Situation | Return value |
|-----------|--------------|
| A line was read | The line, **including** the terminating `\n` (if there is one) |
| Last line without `\n` | The line as it is, without `\n` |
| End of file reached, nothing left | `NULL` |
| Error (`read` fails, invalid `fd`, `BUFFER_SIZE <= 0`, `malloc` fails) | `NULL` |

The returned string is allocated with `malloc`: **the caller must `free` it**.

### What the project teaches

- File descriptors and the `read()` system call
- `static` variables: how a function can keep state between calls
- Dynamic memory: who allocates, who owns and who frees each block
- Writing code that is correct for **any** value of `BUFFER_SIZE` (from `1` to `10000000`)

### Rules followed

- Allowed external functions: `read`, `malloc`, `free`. No `libft`, no `lseek()`, no global variables.
- The size of the `read()` buffer is defined at compile time with `-D BUFFER_SIZE=n` (the header also provides a default value).
- Norm-compliant code (checked with `norminette`).

### Project structure

| File | Role |
|------|------|
| `get_next_line.h` | Header: includes, `BUFFER_SIZE` default and prototypes |
| `get_next_line.c` | `get_next_line()` and the functions that read, extract the line and update the saved remainder |
| `get_next_line_utils.c` | String helpers: length, search for `\n`, join and duplicate |

---

## 🚀 Instructions

### Compilation

The project is not a library: compile the three files together with your own `main`. The buffer size is chosen at compile time.

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
./gnl
```

The same code must work for any size, for example:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1        get_next_line.c get_next_line_utils.c main.c
cc -Wall -Wextra -Werror -D BUFFER_SIZE=9999     get_next_line.c get_next_line_utils.c main.c
cc -Wall -Wextra -Werror -D BUFFER_SIZE=10000000 get_next_line.c get_next_line_utils.c main.c
```

### Usage

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);   /* the line already contains its '\n' */
		free(line);           /* every line belongs to the caller */
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

### Testing

- Compare the output with the original file (`diff` or `cmp`) for several `BUFFER_SIZE` values, including `1` and `10000000`.
- Edge cases: empty file, file with only `\n`, last line without `\n`, very long lines, invalid `fd` (`-1`), a directory, standard input.
- Memory: `valgrind --leak-check=full ./gnl` (no leaks, no invalid reads or writes).
- Failure of `malloc`: simulated by wrapping it at link time (`-Wl,--wrap=malloc`) so that the N-th allocation returns `NULL`; the function must return `NULL` without corrupting memory.
- Community tester: [gnlTester](https://github.com/Tripouille/gnlTester) (all mandatory tests pass; with `BUFFER_SIZE=1` the very long line test can exceed the tester's default 1 second limit when it runs under Valgrind, and passes with a larger timeout).

---

## ⚙️ How it works

`get_next_line()` keeps the text that was read but not yet returned in a **`static` pointer** (the *stash*), which survives between calls. Each call does three things:

1. **Fill the stash.** If the stash does not contain a `\n` yet, call `read()` with a buffer of `BUFFER_SIZE` bytes and append what was read to the stash. Repeat until a `\n` appears or `read()` returns `0` (end of file). Only the newly read chunk is searched for `\n`, so already-scanned text is not scanned again.
2. **Extract the line.** Allocate a new string and copy everything from the beginning of the stash up to (and including) the first `\n`, or to the end if there is none.
3. **Update the stash.** Replace the stash with what comes after the line. If nothing remains, free it and set it to `NULL`, so the next call starts clean and the end of file is detected.

```
file:   "abc\ndef\nghi"        BUFFER_SIZE = 5

call 1: read "abc\nd"  -> stash "abc\nd"   -> returns "abc\n"   stash = "d"
call 2: has no \n      -> read "ef\ngh" -> stash "def\ngh" -> returns "def\n"  stash = "gh"
call 3: no \n          -> read "i"      -> stash "ghi"     -> returns "ghi"    stash = NULL
call 4: read returns 0 -> returns NULL
```

---

## 🧠 Algorithm and data structure

**Data structure: one heap-allocated string (the stash) in one `static` variable.** The only thing that must be remembered between two calls is the text that was read but not yet delivered, and a plain string is enough for that.

**Why this approach**

- It uses exactly the tools the subject allows: `static`, `malloc`, `free` and `read`.
- It works for any `BUFFER_SIZE`: the stash grows as needed, so a line can be much longer than the buffer, and the buffer can be much longer than the file. Reading stops as soon as the first `\n` is found, so a bigger buffer never makes the function read "too much" of the logical stream; the surplus simply stays in the stash for the next call.
- It is simple to reason about memory ownership (see below).

<details>
<summary><b>Memory ownership</b></summary>

<br>

Each block has exactly one owner at any time:

- The **read buffer** is allocated and freed in the same function, on every path (success, end of file and error).
- The **stash** belongs to `get_next_line()`. Whenever it is replaced (after a join, or after the remainder is copied) the old block is freed and the static pointer is reassigned, so it never points to freed memory.
- The **returned line** belongs to the caller.
- On any error (`read` returns `-1`, or a `malloc` fails) everything that was allocated for the call is freed and `NULL` is returned.
</details>

<details>
<summary><b>Error handling</b></summary>

<br>

- `fd < 0` or `BUFFER_SIZE <= 0` return `NULL` immediately.
- If `read()` returns `-1`, the stash is freed and `NULL` is returned.
- If the join of the stash and the new chunk fails (`malloc` returns `NULL`), the reading loop stops and `NULL` is returned, instead of continuing with a lost stash.
- Freeing a `NULL` pointer is safe, which allows a single exit path for these errors.
</details>

<details>
<summary><b>Complexity and limits</b></summary>

<br>

Appending a chunk to the stash allocates a new string and copies the old content, so a single very long line read with a very small `BUFFER_SIZE` costs time proportional to the square of the line length. This is the trade-off of choosing the simplest structure; it is fast for normal files and for reasonable buffer sizes, and still correct in the worst case.

The mandatory part keeps **one** stash, so it follows one file descriptor at a time. Handling several file descriptors at once is the job of the bonus part (one stash per `fd`), which is not part of this submission.
</details>

---

## 📚 Resources

### References

- `man 2 read`, `man 2 open`, `man 2 close`
- `man 3 malloc`, `man 3 free`
- 42 subject: *get_next_line — Reading a line from a fd is way too tedious*
- The 42 Norm (norminette)
- Valgrind documentation ([valgrind.org](https://valgrind.org/docs/manual/manual.html))
- [gnlTester](https://github.com/Tripouille/gnlTester) by Tripouille, used to test the project

### Use of AI

AI (Claude, by Anthropic) was used as a **learning and review tool**, not to write the project for me:

- **Learning the concepts:** explanations of `read`, `open`/`close`, `ssize_t`, `BUFFER_SIZE`, `static` variables and memory ownership with `malloc`/`free`, followed by small practice exercises on each of them.
- **Code review:** it pointed out bugs and Norm issues in my code (wrong indexes, freeing memory that was not allocated, using freed memory, a loop that kept going after a failed `malloc`, function and line limits), and I rewrote the code myself.
- **Testing:** it helped me build test programs (a normal reader, a `malloc` failure simulator and a two-file-descriptor test) and explained how to read the results from Valgrind and gnlTester. These programs are not part of the submission.
- **Documentation:** it helped draft this README, which I reviewed so that I can explain every part.

The design and the code of `get_next_line` are my own, and I can explain each function.