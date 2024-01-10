These examples are from 
http://cs.ecs.baylor.edu/~donahoo/practical/CSockets/practical/

Take a look at the Practical Socket Class. It provides neat abstractions for all the C socket interfaces.

Then look at the TCP Examples.
==========================

TCPEchoClient.cpp - Implements a simple echo client. It first connects to a TCP echo server, and then it sends and receives an echo word supplied by the user on the command line.
TCPEchoServer.cpp - Implements an iterative echo server.
TCPEchoServer-Thread.cpp - Implements a simple threaded echo server, which spawns a new thread for each client. 

Run the TCPEchoServer in one terminal and run the TCPEchoClient in other. The client will first establish the connection to the server and then send the message. 

You can use the following commands

Terminal1:
./TCPEchoServer 1234

Note that the server gets ip address from the host node.

Terminal2:
/TCPEchoClient localhost "here is" 1234

Localhost is a shortcut to say on my machine. It also resolves to a standard ip address 127.0.0.1. Try this: Ping 127.0.0.1. Try and replace TCPEchoServer with TCPEchoServer-Thread.

Try and run the client without starting the server. You will see that the client will get an error.
Connect failed (connect()): Connection refused


Now Look at the UDP Examples
===================================


UDPEchoClient.cpp - Implements a simple echo client. It first connects to a UDP echo server, and then it sends and receives an echo word supplied by the user on the command line.
UDPEchoServer.cpp - Implements a simple echo server.

Terminal1:
./UDPEchoServer  1234

Terminal2:
/UDPEchoClient localhost "here is" 1234

Try and run the client without starting the server. You will see that the client is hanging on.  This is due to a receive call in the code.
If you comment out the lines from 54 to 61 in UDPEchoClient.cpp, compile and run the code again, 

the ./UDPEchoClient localhost "hi there" 1234 will appear to have succeeded even when no server is running. This is precisely the difference between UDP and TCP. 
UDP is unreliable and TCP will let you know if there were errors.