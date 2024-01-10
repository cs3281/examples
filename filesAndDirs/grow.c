#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

// off_t lseek(int fildes, off_t offset, int whence);

int
main(int argc, char *argv[])
{
    int fd = open("file1", O_WRONLY);
    assert(fd >= 0);

    off_t off = lseek(fd, 0, SEEK_END);
    printf("off: %d\n", (int) off);

    char buf[] = "b\n";
    int rc = write(fd, &buf, 2); // ALLOCATING WRITE
    printf("rc: %d\n", rc);

    // close(fd);
    return 0;
}

