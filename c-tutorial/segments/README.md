Segment example
===============

This program shows how the size of the data segment and the size of the bss segment depend on how variables are allocated and whether they are explicitly initialized.

### Building and running

To build and run the program, issue the following:

```bash
 $ cmake .
 $ make
 $ ./main
```

### Static allocation

Static allocation is what happens when you have a global or static variable.

If a statically allocated variable is initialized in your code, it is stored in the *data* segment. For example, in the following snippet:

```c
void foo() {
  static int x = 1;
}
```

storage is allocated in the data segment for the variable x because it is explicitly initialized to the value 1. Beacuse the initial value of x has to be stored in your program, the size of your program will increase by the size of one integer. In other words, the executable file representing your program will be four bytes larger because the initial value of x must be stored.

If a statically allocated variable is *not* initialized in your code, then it is stored in the bss segment. For example, in the following snippet:

```c
void foo() {
  static int y;
}
```

the bss segment contains the variable y. Because y is in the bss segment, the size of your program (that is, the size of the executable file) is not affected by y.

*Dynamic memory* is allocated at runtime at the explicit request of the program, usually using malloc (in C) or new (in C++).

You can read more about memory allocation in C programs [here](http://www.gnu.org/software/libc/manual/html_node/Memory-Allocation-and-C.html).

[This website](http://www.geeksforgeeks.org/memory-layout-of-c-program/) has a nice diagram and explanation of the memory layout of C programs.

### Viewing the sizes of the program segments

After you have compiled the program, you can issue this command to view the sizes of various segments:

```bash
 $ size main
```

Experiment with this program by adding or removing statically allocated variables that either  uninitialized or initialized. Then recompile the code and run the size program to see how the various segment sizes are affected.
