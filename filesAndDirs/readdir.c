#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>



int
main(int argc, char *argv[])
{
    DIR *dp = opendir(".");
    assert(dp != NULL);

    //  struct dirent {
    //    char           d_name[256]; /* filename */
    //    ino_t          d_ino;       /* inode number */
    //    off_t          d_off;       /* offset to the next dirent */
    //    unsigned short d_reclen;    /* length of this record */
    //    unsigned char  d_type;      /* type of file */
    //  }

    struct dirent *d;
    while ((d = readdir(dp)) != NULL) {
	printf("%d %s\n", (int) d->d_ino, d->d_name);
    }

    closedir(dp);
    return 0;
}

