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
  sem_t *produce (0), *consume (0);
  int ret = -1;
  mymessage *shm_segment = 0;
  int shm_fd (-1);

  produce = sem_open ("SHM_example_produce", O_CREAT | O_EXCL,
		      S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 0);

  if (produce == NULL)
    {
      perror ("sem_open");
      ret = -1;
      goto exit;
    }

  consume = sem_open ("SHM_example_consume", O_CREAT | O_EXCL,
		      S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 0);

  if (produce == NULL)
    {
      perror ("sem_open");
      ret = -1;
      goto exit;
    }

  // Open the shared memory segment
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
  // shm_segment = mmap (0, BUFSIZ, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

  if ((shm_segment =
       (mymessage *) mmap (0, sizeof (mymessage), (PROT_READ | PROT_WRITE),
			   MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
    {
      perror ("mmap");
      ret = -1;
      goto exit;
    }

  for (int i = 0; i < 25; ++i)
    {
      int shm_ret (0);
      std::cout << "waiting on produce semaphore \n";
      while (ret = sem_wait (produce))
	{
	  int errorcode = errno;
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
      std::cout << "writing to shm segment\n";
      shm_segment->i = i;
      // snprintf (reinterpret_cast<char *>(shm_segment), BUFSIZ, "bbbbb%d ", i);
      sem_post (consume);
    }

  //make sure consumer has read the last entry;
  sem_wait (produce);

  ret = 0;

exit:
  sem_unlink ("/SHM_example_produce");
  sem_unlink ("/SHM_example_consume");
  shm_unlink ("/SHM_example");
  *(reinterpret_cast < int *>(shm_segment)) = 0;
  return ret;
}