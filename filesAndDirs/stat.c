//The following program calls stat() and displays selected fields in
//       the returned stat structure.

#if 0
  struct stat {
               dev_t     st_dev;         /* ID of device containing file */
               ino_t     st_ino;         /* inode number */
               mode_t    st_mode;        /* file type and mode */
               nlink_t   st_nlink;       /* number of hard links */
               uid_t     st_uid;         /* user ID of owner */
               gid_t     st_gid;         /* group ID of owner */
               dev_t     st_rdev;        /* device ID (if special file) */
               off_t     st_size;        /* total size, in bytes */
               blksize_t st_blksize;     /* blocksize for filesystem I/O */
               blkcnt_t  st_blocks;      /* number of 512B blocks allocated */

               /* Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES. */

               struct timespec st_atim;  /* time of last access */
               struct timespec st_mtim;  /* time of last modification */
               struct timespec st_ctim;  /* time of last status change */

           #define st_atime st_atim.tv_sec      /* Backward compatibility */
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
           };

       Note: the order of fields in the stat structure varies somewhat
       across architectures.  In addition, the definition above does not
       show the padding bytes that may be present between some fields on
       various architectures.  Consult the glibc and kernel source code if
       you need to know the details.

       Note: For performance and simplicity reasons, different fields in the
       stat structure may contain state information from different moments
       during the execution of the system call.  For example, if st_mode or
       st_uid is changed by another process by calling chmod(2) or chown(2),
       stat() might return the old st_mode together with the new st_uid, or
       the old st_uid together with the new st_mode.

       The st_dev field describes the device on which this file resides.
       (The major(3) and minor(3) macros may be useful to decompose the
       device ID in this field.)

       The st_rdev field describes the device that this file (inode)
       represents.

       The st_size field gives the size of the file (if it is a regular file
       or a symbolic link) in bytes.  The size of a symbolic link is the
       length of the pathname it contains, without a terminating null byte.

       The st_blocks field indicates the number of blocks allocated to the
       file, 512-byte units.  (This may be smaller than st_size/512 when the
       file has holes.)

       The st_blksize field gives the "preferred" blocksize for efficient
       filesystem I/O.  (Writing to a file in smaller chunks may cause an
       inefficient read-modify-rewrite.)

       Not all of the Linux filesystems implement all of the time fields.
       Some filesystem types allow mounting in such a way that file and/or
       directory accesses do not cause an update of the st_atime field.
       (See noatime, nodiratime, and relatime in mount(8), and related
       information in mount(2).)  In addition, st_atime is not updated if a
       file is opened with the O_NOATIME; see open(2).

       The field st_atime is changed by file accesses, for example, by
       execve(2), mknod(2), pipe(2), utime(2), and read(2) (of more than
       zero bytes).  Other routines, like mmap(2), may or may not update
       st_atime.

       The field st_mtime is changed by file modifications, for example, by
       mknod(2), truncate(2), utime(2), and write(2) (of more than zero
       bytes).  Moreover, st_mtime of a directory is changed by the creation
       or deletion of files in that directory.  The st_mtime field is not
       changed for changes in owner, group, hard link count, or mode.

       The field st_ctime is changed by writing or by setting inode
       information (i.e., owner, group, link count, mode, etc.).

       POSIX refers to the st_mode bits corresponding to the mask S_IFMT
       (see below) as the file type, the 12 bits corresponding to the mask
       07777 as the file mode bits and the least significant 9 bits (0777)
       as the file permission bits.

       The following mask values are defined for the file type of the
       st_mode field:

           S_IFMT     0170000   bit mask for the file type bit field

           S_IFSOCK   0140000   socket
           S_IFLNK    0120000   symbolic link
           S_IFREG    0100000   regular file
           S_IFBLK    0060000   block device
           S_IFDIR    0040000   directory
           S_IFCHR    0020000   character device
           S_IFIFO    0010000   FIFO

       Thus, to test for a regular file (for example), one could write:

           stat(pathname, &sb);
           if ((sb.st_mode & S_IFMT) == S_IFREG) {
           }
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>

int
main (int argc, char *argv[])
{
  struct stat sb;

  if (argc != 2)
    {
      fprintf (stderr, "Usage: %s <pathname>\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  if (stat (argv[1], &sb) == -1)
    {
      perror ("stat");
      exit (EXIT_FAILURE);
    }

  printf ("ID of containing device:  [%lx,%lx]\n",
	  (long) major (sb.st_dev), (long) minor (sb.st_dev));

  printf ("File type:                ");

  switch (sb.st_mode & S_IFMT)
    {
    case S_IFBLK:
      printf ("block device\n");
      break;
    case S_IFCHR:
      printf ("character device\n");
      break;
    case S_IFDIR:
      printf ("directory\n");
      break;
    case S_IFIFO:
      printf ("FIFO/pipe\n");
      break;
    case S_IFLNK:
      printf ("symlink\n");
      break;
    case S_IFREG:
      printf ("regular file\n");
      break;
    case S_IFSOCK:
      printf ("socket\n");
      break;
    default:
      printf ("unknown?\n");
      break;
    }

  printf ("I-node number:            %ld\n", (long) sb.st_ino);

  printf ("Mode:                     %lo (octal)\n",
	  (unsigned long) sb.st_mode);

  printf ("Link count:               %ld\n", (long) sb.st_nlink);
  printf ("Ownership:                UID=%ld   GID=%ld\n",
	  (long) sb.st_uid, (long) sb.st_gid);

  printf ("Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
  printf ("File size:                %lld bytes\n", (long long) sb.st_size);
  printf ("Blocks allocated:         %lld\n", (long long) sb.st_blocks);

  printf ("Last status change:       %s", ctime (&sb.st_ctime));
  printf ("Last file access:         %s", ctime (&sb.st_atime));
  printf ("Last file modification:   %s", ctime (&sb.st_mtime));

  exit (EXIT_SUCCESS);
}
