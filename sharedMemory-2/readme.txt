This examples shows how to create shared memory segments
It extends the idea of sending two integers between the two processes that you saw in the mqueue_int example.

To run the example, do the following.

$mkdir build
$ cd build
$cmake ..
$ make
$./shm_s (start the sender)
$./shm_r (start the receiver)

You will see in this example that we are creating the shared memory segment with shm_open. Then we set the size of shared memory segment with ftruncate and then map the shared memory segment to an address in the process. The receiver does the same.

the other shared memory example is similar but also uses semaphores to help synchronize the shared memory segment access


