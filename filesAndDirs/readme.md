Basic Abstraction: FILE (regular file)

array of bytes
has low-level name (i.e., a number)
can create/read/write/grow/delete
persistent

Basic Abstraction: DIRECTORY (special type of file)

array of records
maps human-understandable file/dir name to low-level file/dir name 
can create/delete/read (but not write to, directly)


Examples:

CREATION
FILE: Files/creat.c
- look at it
- run it
- strace it


ACCESS (read)
concept of FILE DESCRIPTOR
all open files are tracked in a table of open files
(not shared with other processes, in general)
file descriptor is an integer which each file 
  uses to point to a given open file in that table 
tracked per open file: CURRENT OFFSET
  incremented after a read(), write()

cat file1
strace cat file1
make file bigger
```
  @i = 0 
  while ($i < 5000)
    echo a >> file1
    @ i ++
    echo $i
  end
```
QUESTION: HOW BIG IS THE FILE?
strace cat file1 > /dev/null
  make sure to TRACK CURRENT OFFSET

GROW (write)
FILE: Files/grow.c
how do you grow a file?
use lseek() to seek to the end of the file

TRUNCATE (make file smaller)
FILE: Files/trunc.c (makes it zero-sized) 
  uses O_TRUNC: 
  
 O_TRUNC
If the file exists and is a regular file, and the file is successfully opened O_RDWR or O_WRONLY, its length is truncated to 0 and the mode and owner are unchanged. It will have no effect on FIFO special files or terminal device files. Its effect on other file types is implementation-dependent. The result of using O_TRUNC with O_RDONLY is undefined.

strace trunc
ls -al file1
can also use truncate() system call to make file smaller (not necessarily zero)

DELETE
QUESTION: how to delete a file?
  which system call is used?
strace rm file1
show that it is "unlink"
promise here that by the end of class, they will understand why
(this is one way to make the class have some mystery to it)


RENAME
QUESTION: how to rename a file?
  mv file1 file2
  strace it, see that rename() system call is used


STAT
QUESTION: how to get information about a file?
  strace stat file2

which leads to::
QUESTION: what info should a file system store about a file?

all of this information is stored in a per-file structure called an "inode"
KEY: low-level name of file is thus the "inode number"
(inode is short for index node, because original unix FS stored all inodes
in an array on disk, indexed by this inode number)

Some other interesting things in there:
- SIZE ( and BLOCKS )
- OWNERSHIP
- PERMISSIONS
- ACCESS TIMES
- POINTERS to BLOCKS (though we can't see them)


DIRECTORIES

a directory is just a SPECIAL TYPE of FILE
can create/read/delete (but NOT write to, directly)
stores mapping of human-readable name to low-level name (inode number)

CREATE
strace mkdir foo

REMOVE
strace rmdir foo

can READ as well
FILE: Files/readdir.c (also opendir.c)
  (show how to build this using opendir(), readdir(), closedir())
  QUESTION: what program is this?? (answer: it's "ls")

Some special directories:
ROOT: "/" at the top of the tree
CURRENT: "."
PARENT: ".."

pathnames:
ABSOLUTE: /x/y/z
  root / directory / directory / ... / file or directory
RELATIVE: foo/bar.c
  directory / directory / ... / file or directory
key notion: SEPARATOR 

relative: each process has notion of CURRENT WORKING DIRECTORY
  getcwd() to see it
  
ls -ali /
  show root inode number is always 2

Revisiting OPEN
  What does OPEN() do?
  fd = open("/a/b/c/file1", O_RDONLY);
    internally, starts at root directory (or cwd if relative)
    must TRAVERSE root directory, 'a' directory, 'b', 'c', finally finds
    'file1' and reads its inode 
  fd is then used to point to 'file structure' for this open file
    tracks on a per-process basis where the CURRENT FILE OFFSET is


LINKS (HARD LINK):
  NOT a special type of file
    (but you might think it is)
  how to create? 
    ln file1 file2
    strace it! (show that link() is called)
  now:
    cat file1
    cat file2
    ls -ali file1 file2
    -> see, same inode number!
    stat file1 file2 
    -> see LINK COUNT
    rm file1
    cat file2: WILL IT WORK?
    stat file2
    -> see LINK COUNT again
    rm file2

And now you know: why REMOVE is done by UNLINKing a file
  each unlink removes a single reference to a file
  when final unlink occurs, file is removed from file system

LINKS (SYMBOLIC):
  A special file type
  ln -s file1 file2
  strace to show symlink() system call
  ls -al file1 file2
    SHOW HOW IT IS A SPECIAL FILE TYPE
    NOTE FILE SIZE of file2
    (it is 5 bytes: can anyone guess why?)
  rm file1
  cat file2
  dangling pointer
  why needed?
  -> to point to files in other "file systems" !
  (inode numbers are only unique INSIDE of a file system, not ACROSS)
   which brings us to...

MOUNTING FILE SYSTEMS
  each file system is stored in a "VOLUME"
  looks like a disk (but could be part of a disk, a partition)
  but all stitched together into SINGLE TREE of all file systems

  type "mount"
  show root directory
  show how another disk is mounted on "/scratch.1"

  can use this to assemble full file-system tree!
  (instead of having c: and d: and e: on desktop)







