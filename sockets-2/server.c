/* 
   Purpose of this code is to demonstrate the use of low-level Socket API in C
   language. This code base is used on the server-side.
*/


#include <stdio.h>       // for I/O
#include <string.h>      // basic string API
#include <unistd.h>      // for getpid (), getopt ()
#include <stdlib.h>      // for atoi, etc
#include <sys/types.h>   // for various data types
#include <sys/socket.h>  // for socket API
#include <netinet/in.h>  // for sockaddr_in
#include <arpa/inet.h>   // for inet_addr


/* 
   Server code uses a different structure than the client. A server is a
   passive entity listening for incoming connection requests from clients. 
*/

// server main function
int main (int argc, char *argv[])
{ 
    // first declare the variables
    u_short port_num;         // port number is unsigned short
    int listen_sock;          // socket handle for the listening port
    int conn_sock;            // socket handle for accepted connection
    struct sockaddr_in server_addr;  // required for bind (See below)
    int opt;          // used in command line arg parsing

    /* Now do command line parsing. We support a command line that is of the
       form:

       server [-p <port num>] [-h]
    */
    port_num = 10000; // set default port num of server
    
    // getopt is a function that provides command line parsing. A : after the
    // letter indicates that there is an argument following the letter.
    while ((opt = getopt (argc, argv, "hp:")) != -1) {
        switch (opt) {
        case 'p':  // port num
            port_num = atoi (optarg);  // atoi converts string to number
            break;
        case 'h': // help menu
        default:
            printf ("Usage: %s [-h] [-p <port num>]\n",
                    argv[0]);
            return -1;
        }
    }

    // now that we have successfully parsed the command line, if any, we now
    // proceed to initialize the socket and connect to the server
    printf ("Server listening on port num: %d\n", port_num);


    // STEP (1)
    // initialize the socket for the port we will listen on.
    //
    // A socket is a handle created by the operating system that
    // serves as the "end-point" through which network I/O can be
    // performed. Note, however, that simply creating a socket handle
    // does not immediately allow the server to start receiving client
    // requests. As shown in the different steps below, the socket
    // handle must first be associated with the network interface of
    // the machine. Only then can you start listening for incoming
    // requests. When a client sends a "connection establish" request,
    // the server will accept it using the "accept" command shown
    // below. Therefter the client and server exchange messages until
    // the session is over in which case both parties close the socket
    // handles.
    listen_sock = socket (AF_INET,      // use IPv4 family
                          SOCK_STREAM,  // full duplex byte stream
                          0);           // protocol type (TCP)

    // error checking
    if (listen_sock == -1) {
        perror ("socket creation failed");
        return -1;
    }
                          
    // STEP (2)
    // now we need to bind the socket handle to the network interface
    // over which we intend to receive incoming requests. The use of
    // INADDR_ANY shown below indicates that we will receive request
    // over any network interface that this machine has.  If we want to accept
    // conections over a specific interface, then we need to provide that
    // specific interface. For now, we choose to allow incoming traffic on any
    // interface we have on our machine.
    //
    //
    /* We need to populate the following structure in order to use the socket
       API for the Internet protocols TCP/IP and UDP/IP

       struct sockaddr_in {
         sa_family_t    sin_family; // address family: AF_INET
         in_port_t      sin_port;   // port in network byte order
         struct in_addr sin_addr;   // internet address
       };

    
       where, the Internet address specified in the struct in_addr is of the
       following form: 
       
       struct in_addr {
         uint32_t       s_addr; // address in network byte order
       }
    */
    
    // (2a) first make sure we zero out the structure as we do not want any
    // garbage values in this structure.  The sizeof () function returns the
    // number of bytes consumed by the data structure that we are trying to
    // zero out. 
    memset (&server_addr, 0, sizeof (struct sockaddr_in));

    // (2b) set the fields of the structure

    // first indicate the addressing family. We choose IP version 4.
    // Note that although we used AF_INET to the socket call, this has to be
    // specified in this structure too. We must make sure that the same value
    // used in the socket call should be used here.
    server_addr.sin_family = AF_INET;   // IPv4 family

    // see how we use INADDR_ANY to indicate we will receive requests
    // over all interfaces. Also note how we use the function
    // "htonl". This function converts data from native type to
    // network type. The "htonl" indicates conversion of a "long" from
    // host to network byte order.  The architecture of the CPU on our machine
    // could be little endian or big endian. Intel chips are little-endian
    // machines whereas Sun Sparc or PowerPC chips are big endian. 
    //
    // The protocol standardizing committee decided to use "big-endian" as the
    // byte order of any data that goes over the network. Therefore, every time
    // any data must be shipped has to be converted between the host format
    // (which could be little or big endian) and the network format (which is
    // big endian).  The OS provides utility functions called "htonl" (and
    // correspondingly "ntohl") to convert a "long" value in host byte order to
    // "long" value in network byte order (note that "ntohl" will do the
    // reverse action). For "short" data type we have "htons" and
    // "ntohs". Nothing needs to be done for "char" 
    server_addr.sin_addr.s_addr = htonl (INADDR_ANY);

    // indicate what port number we will be listening on
    // once again see how we convert the data into network byte
    // order. This time we use htons since port_num is a "short"
    server_addr.sin_port = htons (port_num);
    
    // (2c) Now make the association of the socket to the address via the
    // "bind" call

    /*
      The bind system call below takes the listen socket and the populated
      struct as parameters.  The 2nd parameter, however, is a generic data
      structure. This generic nature of the data structure stems from the need
      to support a wide range of protocols beyond the Internet protocols. Since
      these APIs are all in C language, there was no way to create a class
      hierarchy as in object oriented languages, and be able to use the right
      type via run time type identification. Thus, the only way is to use the
      cast operator as we will see below.

         struct sockaddr {
           sa_family_t sa_family;
           char        sa_data[14];
         }

      Note how the sa_family field in the generic data structure maps to the
      protocol-specific data structure.
    */

    int bind_status;
    bind_status =
        bind (listen_sock,    // this was the listen socket handle we created earlier
              // the second arg is supposed to be of type "sockaddr *" which is
              // a typedef declared in the OS headers to  represent a generic
              // network family. However, we  are using the IPv4 family and
              // hence had initialized the "sockaddr_in" structure. Therefore,
              // now we must cast the sockaddr_in to sockaddr, which is the
              // parameter used by the bind call.
              //
              // In C language, therefore, we cast it to the right type
              (struct sockaddr *)&server_addr,
              // indicate the size of the structure
              sizeof (struct sockaddr));

    // error checking
    if (bind_status == -1 ) {
        perror ("bind failed");
        return -1;
    }

    // STEP 3: Now indicate to the OS that the server is ready to
    // start listening for incoming connection establishment requests
    // from clients. The second parameter tells the OS as to how many
    // outstanding incoming requests to queue up at a time.
    int listen_status;
    listen_status = listen (listen_sock, // our listen sockethandle
                            5);          // backlog parameter (don't worry)

    // error checking
    if (listen_status == -1) {
        perror ("listen failed");
        return -1;
    }

    // STEP 4: Now accept any incoming connection. Note that we are a
    // server and hence we are supposed to accept connections and serve 
    // the client requests. Most likely the service we provide is a long 
    // living service. Therefore, we do the following logic inside an
    // infinite loop
    for (;;) {
        printf ("Server: WAITING TO ACCEPT A NEW CONNECTION\n");

        // the accept command shown below actually does the job of
        // the TCP/IP 3-way handshaking protocol when a client
        // requests a connection establishment.
        //
        // Note that the accept command creates a new socket handle as the
        // return value of "accept". Understand that this is necessary because
        // in order to serve several clients simultaneously, the server needs
        // to distinguish between the handle it uses to listen for new
        // connections requests and the handle it uses to exchange data with
        // the client. Thus, the newly created socket handle is used to do the
        // network I/O with client whereas the older socket handle  continues
        // to be used for listening for new connections. 
        conn_sock = accept (listen_sock, // our listen sock handle
                            0,  // we don't care about details of client
                            0); // hence length = 0

        // error checking
        if (conn_sock == -1) {
            perror ("accept failed");
            return -1;
        }

        printf ("Server: ACCEPTED A NEW CONNECTION\n");

        // The I/O with client starts here. However, if we do data exchange
        // with the client here itself, then the server will be blocked until
        // this client is done. We cannot then achieve concurrency doing I/O
        // with client while simulataneously listening for new client
        // connections.  A way out is to either fork of a child process to deal
        // with the client or spawn a thread or hand over the client  request
        // to a thread while the main thread keeps listening for new requests.
        //
        // If you do a fork, the child will do the data communication with the
        // client; thus the child does not need the listening socket and can
        // close it right away. On the other hand, the original parent does not
        // need the newly created communication socket, and can close it. 

        // In this case we just showcase an interative server wherein
        // a server serves one client at a time.

        // define a buffer to hold client data
        char data_buff [1024];
        // zero out its contents so that it does not have any garbage values
        memset (data_buff, 0, sizeof (data_buff)); 

        // block until something is received
        int recv_status
            = recv (conn_sock,  // client I/O to be done with new socket
                                // second parameter is a buffer into which you
                                // receive data. Linux needs a (void *) buffer
                    (void *)data_buff,
                    // third parameter is the size of the buffer
                    sizeof (data_buff),
                    // last parameter is a flag that we
                    // will ignore.
                    0);

        // network I/O is tricky. Sometime you do not receive all the
        // bytes you have requested. I am not going to check for this
        // condition. On Linux you can force the kernel to not return
        // until all the requested bytes are read by using a special
        // flag.
        if (recv_status == -1) {
            perror ("recv failed");
            return -1; // note we should really cleanup but we ignore it
        }
        
        printf ("Server: Received data: %s\n", data_buff);

        // we just send this same data back to client :-)

        int send_status = 
            send (conn_sock, // I/O is done on this socket handle
                  // second parameter is the buffer you send
                  (void *) data_buff,
                  // 3rd param is the length of the buffer
                  sizeof (data_buff),
                  // we ignore the flags argument
                  0);
        
        // error checking
        if (send_status == -1) {
            perror ("send failed");
            return -1; // note we should really cleanup but we ignore it
        }

        // STEP 6: close the new I/O handle because we are done with the client
        // with just one round trip communication. But note that this need not
        // be the case in real world applications. Thus, a server will continue
        // communicating until the client closes the connection.
        (void) close (conn_sock);


        // we make a policy decision that if we receive the string "exit" 
        // from the client, we are going to quit ourselves.
        if (strcmp (data_buff, "exit") == 0) {
            printf ("Server is EXITING\n");
            break;
        }
    } // end of for loop


    // cleanup the listening endpoint
    (void) close (listen_sock);

    return 0;
}
