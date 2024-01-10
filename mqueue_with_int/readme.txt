This example shows a message queue receiver and a message queue sender.
However, instead of sending characters it is sending integers.

The sending is done by

```
// here i is an integer
int code= mq_send (mq, (const char*)&i, sizeof(i), i);
```

And receive is done via

```
// here message is an integer
  ssize_t received = mq_receive (mq, (char *) &message, sizeof(message),
                                     &prio);
```

Build the project using the following

$ chmod +x build.sh
$ ./build.sh
$ ./mqueue_s (run in 1 terminal)
$ ./mqueue_r (run in second terminal)
