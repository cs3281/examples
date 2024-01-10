#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

int
main(int argc, char *argv[])
{
    int rc = creat("file1", S_IRWXU); //open("file1",O_CREAT,S_IRWXU);
    assert(rc >= 0);
    return 0;
}

