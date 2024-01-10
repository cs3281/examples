#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    // truncate() system call or open (O_TRUNC)
    int fd = open("file1", O_WRONLY|O_CREAT|O_TRUNC);
    assert(fd >= 0);
    return 0;
}

