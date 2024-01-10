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
  config.mq_msgsize = sizeof(unsigned int);
  config.mq_curmsgs = 0;

  mqd_t mq = mq_open ("/MyCoolMQ_int", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR |
                                                        S_IRGRP | S_IROTH, &config);

  if (mq == -1)
    {
      perror ("mq_open");
      return -1;
    }

  for (unsigned int i = 0; i < 25; ++i)
    {

      std::cerr << "Sending message " << i << "size of message is "
                <<sizeof(i)<<std::endl;
      int code= mq_send (mq, (const char*)&i, sizeof(i), i);


      if(code==-1)
        {
          perror ("mq_send");
          return -1;
        }

      sleep (1);
    }
  mq_close(mq);
  mq_unlink ("/MyCoolMQ");
}
