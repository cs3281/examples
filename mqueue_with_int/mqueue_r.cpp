#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>

int main (int argc, char **argv)
{
//  mqd_t mq = mq_open ("/MyCoolMQ", O_RDONLY | O_CREAT);


  mqd_t mq = mq_open ("/MyCoolMQ_int", O_RDONLY , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (mq == -1)
    {
      perror ("mq_open");
      return -1;
    }

  struct mq_attr attr;
  mq_getattr (mq, &attr);



  for ( ;; )
    {
      unsigned int prio;
      unsigned int message;
      ssize_t received = mq_receive (mq, (char *) &message, sizeof(message),
                                     &prio);

      if (received == -1)
        {
          perror ("mq_receive");
          return -1;
        }

      if (prio == 24) break;

      std::cerr << "Received " << received << " bytes at " << prio <<
                                                                   "priority:"
                                                                       " "
                                                                       "message:" <<
                                                                           message << std::endl;
    }
  mq_close(mq);
  mq_unlink ("/MyCoolMQ");

}
