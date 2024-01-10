#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int
main(int argc, char *argv[])
{
    int rc = unlink("file1");
    assert(rc == 0);
    return 0;
}





