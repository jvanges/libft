*This project has been created as part of the 42 curriculum by jvan-ges.*

# Libft

## Description

Libft is my first own C library. The goal of the project is to understand how
the standard C functions work by reimplementing them, and to build a set of
general-purpose tools that can be reused in later 42 projects.

The library is compiled into a static archive, `libft.a`, created at the root
of the repository with the `ar` command. It is made of three parts:

1. **Libc functions**: reimplementations of standard functions, with the same
   prototypes and behaviors as the originals, prefixed with `ft_`.
2. **Additional functions**: string, conversion and output helpers that are
   either missing from the libc or exist in a different form.
3. **Linked list**: a small API built on the `t_list` structure.

### Library contents

#### Part 1 - Libc functions

| Category | Functions |
|---|---|
| Character checks | `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint` |
| Character conversion | `ft_toupper`, `ft_tolower` |
| Strings | `ft_strlen`, `ft_strlcpy`, `ft_strlcat`, `ft_strchr`, `ft_strrchr`, `ft_strncmp`, `ft_strnstr`, `ft_strdup` |
| Memory | `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_calloc` |
| Conversion | `ft_atoi` |

Notes:
- The character classification functions return `1` if the character matches
  the tested class and `0` otherwise.
- `ft_calloc` returns a unique pointer that can be passed to `free()` when
  `nmemb` or `size` is `0`.
- `ft_calloc` and `ft_strdup` use `malloc`. All other functions in this part
  rely on no external function.

#### Part 2 - Additional functions

| Function | Description |
|---|---|
| `ft_substr` | Returns a newly allocated substring of `s`, starting at index `start`, of at most `len` characters. |
| `ft_strjoin` | Returns a newly allocated string that is the concatenation of `s1` and `s2`. |
| `ft_strtrim` | Returns a newly allocated copy of `s1` with the characters of `set` removed from the beginning and the end. |
| `ft_split` | Splits `s` using the character `c` as delimiter. Returns a NULL-terminated array of independently allocated strings. |
| `ft_itoa` | Returns a newly allocated string representing an integer, negative numbers included. |
| `ft_strmapi` | Applies `f` to each character of `s` (index and character as arguments) and returns a new string of the results. |
| `ft_striteri` | Applies `f` to each character of `s`, passing its index and its address so it can be modified. |
| `ft_putchar_fd` | Writes a character on the given file descriptor. |
| `ft_putstr_fd` | Writes a string on the given file descriptor. |
| `ft_putendl_fd` | Writes a string followed by a newline on the given file descriptor. |
| `ft_putnbr_fd` | Writes an integer on the given file descriptor. |

Notes:
- Functions that allocate memory return `NULL` if an allocation fails.
- The array returned by `ft_split` is released by calling `free()` on each
  string, then on the array itself.

#### Part 3 - Linked list

The list uses the following structure, declared in `libft.h`:

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
} t_list;
```

| Function | Description |
|---|---|
| `ft_lstnew` | Creates a new node with the given content; `next` is set to `NULL`. |
| `ft_lstadd_front` | Adds a node at the beginning of the list. |
| `ft_lstsize` | Counts the number of nodes in the list. |
| `ft_lstlast` | Returns the last node of the list. |
| `ft_lstadd_back` | Adds a node at the end of the list. |
| `ft_lstdelone` | Frees the content of a node with `del`, then frees the node itself (not the next one). |
| `ft_lstclear` | Deletes and frees a node and all its successors, then sets the list pointer to `NULL`. |
| `ft_lstiter` | Applies `f` to the content of each node. |
| `ft_lstmap` | Applies `f` to the content of each node and builds a new list from the results. `del` is used to clean up if an allocation fails. |

### Technical choices

- No global variables.
- Helper functions are declared `static` to restrict their scope to their file.
- The library is built with `ar` (not `libtool`) and every file is compiled
  with `cc -Wall -Wextra -Werror`.
- The Makefile does not relink when nothing has changed.

## Instructions

### Compilation

```sh
make          # builds libft.a at the root of the repository
make clean    # removes the object files
make fclean   # removes the object files and libft.a
make re       # rebuilds everything from scratch
```

### Usage

Include `libft.h` in your source file and link against the library:

```c
#include "libft.h"

int	main(void)
{
	ft_putendl_fd("Hello, libft!", 1);
	return (0);
}
```

```sh
cc -Wall -Wextra -Werror main.c -L. -lft -o demo
./demo
```

## Resources

- `man 3` pages of the reimplemented functions (`strlen`, `memmove`,
  `strlcpy`, `strlcat`, `calloc`, ...)
- The C standard library documentation
- Discussions with other students (peer learning)

### AI usage

AI was used to help write this README and to reformat my functions so they
comply with the 42 Norm. The implementation of the functions themselves was
written by me.
