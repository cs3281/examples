#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>

int main (int argc, char **argv)
{
//  mqd_t mq = mq_open ("/MyCoolMQ", O_RDONLY | O_CREAT);

  struct mq_attr config;
  config.mq_flags = 0;
  config.mq_maxmsg = 3;
  config.mq_msgsize = 5;
  config.mq_curmsgs = 0;

  mqd_t mq = mq_open ("/MyCoolMQ", O_RDONLY |O_CREAT, S_IRUSR | S_IWUSR |
                                                   S_IRGRP | S_IROTH,&config);

  if (mq == -1)
    {
      perror ("mq_open");
      return -1;
    }

  struct mq_attr attr;
  mq_getattr (mq, &attr);

  char *buf = new char[attr.mq_maxmsg];

  for ( ;; )
    {
      unsigned int prio;
      ssize_t received = mq_receive (mq, buf, attr.mq_msgsize, &prio);

      if (received == -1)
        {
          perror ("mq_receive");
          return -1;
        }

      if (prio == 24) break;

      std::cerr << "Received " << received << " bytes at " << prio << "priority: " << buf << std::endl;
    }
  mq_close(mq);
  mq_unlink ("/MyCoolMQ");

}
