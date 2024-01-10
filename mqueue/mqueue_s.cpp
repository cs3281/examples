#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

int main (int argc, char **argv)
{
  struct mq_attr config;
  config.mq_flags = 0;
  config.mq_maxmsg = 3;
  config.mq_msgsize = 5;
  config.mq_curmsgs = 0;

  mqd_t mq = mq_open ("/MyCoolMQ", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, &config);

  if (mq == -1)
    {
      perror ("mq_open");
      return -1;
    }

  for (size_t i = 0; i < 25; ++i)
    {
      char buf[5];
      int count = snprintf (buf, 5, "%lu ", i);
      std::cerr << "Sending message " << buf << std::endl;
      mq_send (mq, buf, count, i);

      sleep (1);
    }
  mq_close(mq);
  mq_unlink ("/MyCoolMQ");
}
