#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

struct mymessage
{
  int i;
};


int
main (int argc, char **argv)
{
  int ret (-1);
  sem_t *produce (0), *consume (0);
  int shm_fd (-1);
  mymessage *shm_segment (0);

  produce = sem_open ("SHM_example_produce", 0);

  if (produce == NULL)
    {
      perror ("sem_open");
      goto exit;
    }

  consume = sem_open ("SHM_example_consume", 0);

  if (produce == NULL)
    {
      perror ("sem_open");
      goto exit;
    }

  // Open the shared memory segment
  shm_fd = shm_open ("/SHM_example",
		     O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (shm_fd < 0)
    {
      perror ("shm_open");
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
  if ((shm_segment =
       (mymessage *) mmap (0, sizeof (mymessage), (PROT_READ | PROT_WRITE),
			   MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
    {
      perror ("mmap");
      ret = -1;
      goto exit;
    }


  // signal the produce that we're ready
  printf ("Signaling producer...\n");
  sem_post (produce);

  for (int i = 0; i < 26; ++i)
    {
      int shm_ret (0);
      printf ("waiting on consume...\n");
      while (ret = sem_wait (consume))
	{
	  int errorcode = errno;
	  printf ("Loop\n");

	  switch (errorcode)
	    {
	    case EINTR:
	    case EAGAIN:
	      continue;
	    default:
	      perror ("sem_wait");
	      ret = -1;
	      goto exit;
	    }
	}
      printf ("released on semaphore...\n");


      printf ("read %d\n", (shm_segment)->i);
      printf ("Signaling producer\n");
      sem_post (produce);
      if ((shm_segment)->i == 24)
	{
	  printf ("Producer is finished!\n");
	  break;
	}
    }
  printf ("Loop finished\n");

  *(reinterpret_cast < int *>(shm_segment)) = 0;

  ret = 0;

exit:
  sem_unlink ("/SHM_example_produce");
  sem_unlink ("/SHM_example_consume");
  shm_unlink ("/SHM_example");
  return ret;
}