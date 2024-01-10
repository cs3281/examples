#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>


struct mymessage
{
    int i;
};


int main (int argc, char **argv)
{
  mymessage *shm_segment = 0;
  int shm_fd (-1);

  int ret=0;

  // Open the shared memory segment. Note that it has been opened in Read
  // write mode
  shm_fd = shm_open ("/SHM_example",
                     O_CREAT | O_EXCL | O_RDWR,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (shm_fd < 0)
    {
      perror ("shm_open");
      ret = -1;
      goto exit;
    }

  // Set the size of the shared memory segment
  if (ftruncate (shm_fd, sizeof (mymessage)) != 0)
    {
      perror ("ftruncate");
      ret = -1;
      goto exit;
    }

  // Map the shared memory segment into memory

  // The PROT_READ and PROT_WRITE Allows you to both read and write from the
  // shared memory segment. This is the page level access that mmap controls.

  // the map shared
  if ((shm_segment =
           (mymessage *) mmap (0, sizeof (mymessage), (PROT_READ | PROT_WRITE),
                               MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
    {
      perror ("mmap");
      ret = -1;
     return ret;
    }



  for (size_t i = 0; i < 25; ++i)
    {

      std::cerr << "Sending message " << i << std::endl;
      shm_segment->i=i;

    }
exit:
  return ret;
}
