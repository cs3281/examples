//This code has been sourced from an example here: http://www.ibm.com/developerworks/linux/library/l-ubuntu-inotify/index.html
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int
main (int argc, char **argv)
{
  if (argc != 2)
    {
      printf ("usage: %s \"directory to watch\"\n", argv[0]);
      exit (-1);

    }
//check if the directory is valid
  struct stat s;
  int err = stat (argv[1], &s);
  if (-1 == err)
    {
      if (ENOENT == errno)
	{
	  printf ("%s does not exist\n", argv[1]);
	  exit (1);
	}

      else
	{
	  perror ("stat");
	  exit (1);
	}
    }
  else
    {
      if (S_ISDIR (s.st_mode))

	{
	  printf ("watching %s for changes \n", argv[1]);
	}
      else
	{
	  printf ("%s exists but is not a directory\n", argv[1]);
	  exit (1);
	}
    }

  int length, i = 0;
  int fd;
  int wd;
  char buffer[BUF_LEN];
  fd = inotify_init ();

  if (fd < 0)
    {
      perror ("inotify_init");
    }

  wd = inotify_add_watch (fd, argv[1], IN_MODIFY | IN_CREATE | IN_DELETE);


  while (1)
    {
      printf ("going to block\n");
      length = 0;
      i = 0;
      length = read (fd, buffer, BUF_LEN);

      if (length < 0)
	{
	  perror ("read");
	}

      while (i < length)
	{
	  struct inotify_event *event = (struct inotify_event *) &buffer[i];
	  if (event->len)
	    {
	      if (event->mask & IN_CREATE)
		{
		  if (event->mask & IN_ISDIR)
		    {
		      printf ("The directory %s was created.\n", event->name);
		    }
		  else
		    {
		      printf ("The file %s was created.\n", event->name);
		    }
		}
	      else if (event->mask & IN_DELETE)
		{
		  if (event->mask & IN_ISDIR)
		    {
		      printf ("The directory %s was deleted.\n", event->name);
		    }
		  else
		    {
		      printf ("The file %s was deleted.\n", event->name);
		    }
		}
	      else if (event->mask & IN_MODIFY)
		{
		  if (event->mask & IN_ISDIR)
		    {
		      printf ("The directory %s was modified.\n",
			      event->name);
		    }
		  else
		    {
		      printf ("The file %s was modified.\n", event->name);
		    }
		}
	    }
	  i += EVENT_SIZE + event->len;
	}
    }

  (void) inotify_rm_watch (fd, wd);
  (void) close (fd);

  exit (0);
}
