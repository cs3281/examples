#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
int
main (int argc, char *argv[])
{
  int status;
  int f1=open("newfile.txt",O_RDWR|O_CREAT,666);
  printf("the error was %d\n",errno);

  printf("STDIN file descriptor %d\n",STDIN_FILENO);
  printf("STDOUT file descriptor %d\n",STDOUT_FILENO);
  printf("STDERR file descriptor %d\n",STDERR_FILENO);
  printf("file descriptor %d\n",f1);
  write(f1,"text\n",strlen("text\n"));
  char buffer[10];

  // You will have to reset the pointer to the beginning.
  
  lseek(f1,0,SEEK_SET);
  int error=read(f1,&buffer[0],10);
  buffer[9]='\0';
  printf("error %d %s\n",error,buffer);

  write(STDOUT_FILENO,"text\n",strlen("text\n"));
  close(f1);
}
