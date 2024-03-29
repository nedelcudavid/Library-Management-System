# Library Management System
## Copyright 2022 Nedelcu Andrei-David

This project aims to provide a comprehensive library management system. Below are the roles of the files constituting the solution:

* **hashtable.c** and **.h**: Provide the implementation of a resizable generic hash table interface.

* **entities.c** and **.h**: Model the "objects" central to this program, such as users and books. They expose various general functions including initialization, deallocation, comparison (for sorting), display, and the `get_rating()` function for books.

* **commands.c** and **.h**: Contain the actual implementation of the command execution module, with one function for each command. For parsing command arguments, the `get_next_arg()` function from **utils.c/h** is used. This function is employed initially in `main` to extract the command name for calling the desired function, and subsequently within specialized functions to extract each relevant parameter of the command.

* **utils.c** and **.h**: Contribute to the structure of the solution with two functions:
  - `dup_str()`, representing a C99 implementation of the `strdup()` function.
  - `get_next_arg()`, which takes a position in the string where the command is stored and a buffer. It extracts the first argument from this position, updates the position to the argument extracted, and copies the argument into the buffer. If an argument is successfully extracted, the function returns a pointer to the buffer passed as a parameter; otherwise, it returns NULL.

The following steps were undertaken to solve the problem:

1. Allocation of tables for books and users (in **main.c**).
2. Allocation of the buffer for the entire command.
3. Reading the next command.
4. Extracting the command name and calling the corresponding function for execution, also displaying any relevant messages.
5. If the function is not "EXIT," execution returns to step 3.
6. If the function is "EXIT," the ranking is displayed.
7. Freeing the memory allocated for the tables of books and users, as well as the command buffer.
8. Halting program execution.
