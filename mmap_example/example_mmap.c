#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <stdio.h>

//Find out the eize of the file
size_t sizeFileFunc(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;   
}

int main(int argc, char** argv) {
     //argv[1] is the name of the file to be read using mmap
    if(argv[1] == NULL){
    	printf("Need to supply the file name to be read!!\r\n");
        exit(EXIT_FAILURE); 
    }	
    size_t fsize = sizeFileFunc(argv[1]);
    //Open file
    int fd = open(argv[1], O_RDONLY, 0);
    assert(fd != -1);
    //Execute mmap . Note that the alternative would have been to use read instead of mmap.
    void* data = mmap(NULL, fsize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    assert(data != NULL);
    
    //Write the mmapped data to stdout (= FD #1)
    write(1, data, fsize);
    
    //Cleanup
    int rc = munmap(data, fsize);
    assert(rc == 0);
    close(fd);
}
