#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

// off_t lseek(int fildes, off_t offset, int whence);

int
main(int argc, char *argv[])
{
    int fd = open("/tmp/file", O_WRONLY);
    assert(fd >= 0);

    // lock
    int i;
    for (i = 0; i < atoi(argv[2]); i++) {
	assert(flock(fd, LOCK_EX) == 0);
	(void) lseek(fd, 0, SEEK_END);
	char buf[100];
	sprintf(buf, "%s\n", argv[1]);
	int rc = write(fd, buf, strlen(buf)); 
	assert(rc == strlen(buf));
	// unlock
	assert(flock(fd, LOCK_UN) == 0);
    }

    close(fd);
    return 0;
}

