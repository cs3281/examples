/* 
   Purpose of this code is to demonstrate the use of low-level Socket API in C
   language. This code base is used on the client-side.
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
   Client code uses a slightly different structure than the server. Instead of
   listening and accepting connections as in the server, it will actively
   connect to the server.
*/

// client main function
int main (int argc, char *argv[])
{ 
    // first declare the variables
    u_short port_num;         // port number is unsigned short
    char dotted_form [15];    // IP addr of server in dotted form
    int conn_sock;            // socket handle for connection
    struct sockaddr_in server_addr;  // required for connect (See below)
    int opt;          // used in command line arg parsing

    /* Our client will need to connect to a server running at some IP address
       and port number.

       To find out what is the ip address of the server, open a shell on the OS
       where the server is being deployed and type: 

           ipconfig /all (this works on windows) 
       or
           ifconfig (works on Linux, Mac)
       or
           netstat -i (works on Linux, Mac)
    */

    /* Now do command line parsing. We support a command line that is of the
       form:

       client [-a <IP addr>] [-p <port num>] [-h]
    */
    port_num = 10000; // set default port num of server to connect to
    strcpy (dotted_form, "127.0.0.1"); // connect to localhost by default
    
    // getopt is a function that provides command line parsing. A : after the
    // letter indicates that there is an argument following the letter.
    while ((opt = getopt (argc, argv, "a:hp:")) != -1) {
        switch (opt) {
        case 'a': // IP address in dotted decimal form
            strcpy (dotted_form, optarg);
            break;
        case 'p':  // port num
            port_num = atoi (optarg);  // atoi converts string to number
            break;
        case 'h': // help menu
        default:
            printf ("Usage: %s [-h] [-a <IP addr>] [-p <port num>]\n",
                    argv[0]);
            return -1;
        }
    }

    // now that we have successfully parsed the command line, if any, we now
    // proceed to initialize the socket and connect to the server
    printf ("Client is connecting to server at IP addr: %s and port num: %d\n",
            dotted_form, port_num);

    //
    // STEP (1)
    // initialize the socket for data communications with the server. On
    // success, the system call returns a file descriptor for the socket
    // handle.
    //
    // A socket is a handle created by the operating system that
    // serves as the "end-point" through which network I/O can be
    // performed. Note, however, that simply creating a socket handle
    // does not immediately allow the client to start sending client
    // requests. As shown in the different steps below, the socket
    // handle must first connect to a server. Therefter the client and server
    // exchange messages until the session is over in which case both parties
    // close the socket handles.
    conn_sock = socket (AF_INET,      // use IPv4 family
                        SOCK_STREAM,  // full duplex byte stream
                        0);           // protocol type (TCP)

    // error checking
    if (conn_sock == -1) {
        perror ("socket call failed");
        return -1;
    }
                          
    // STEP (2)
    // Now we must connect to the server. 
    // A client is considered to be the active entity because it
    // initiates a dialog with the server. On the other hand the
    // server is a passive entity
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
    
    // (2a) first make sure we zero out the structure. If we do not do this, it
    // may contain garbage values in the uninitalized structure. Notice, how we
    // use the sizeof () function to select the right number of bytes to zero
    // out. 
    memset (&server_addr, 0, sizeof (struct sockaddr_in));

    // (2b) populate the fields of the structure
    // first indicate the addressing family. We choose IP version 4.
    // Note that although we used AF_INET to the socket call, this has to be
    // specified in this structure too. We must make sure that the same value
    // used in the socket call should be used here.
    server_addr.sin_family = AF_INET;   // IPv4 family

    // Populate the network address of the server. Note that the address we
    // used was a dotted decimal string that is understandable by humans. But
    // this is not understood by the network API. It needs the network address
    // in a binary format. That is why we use the "inet_ntoa" utility
    // function. Note that the populated value is already in network byte
    // order. So you do not yet again convert it.
    //
    // But, what exactly is a network byte order and why is this needed? It
    // turns out that machines can have either the Little Endian or Big Endian
    // architecture (defined by whether the address of a Word points to its 
    // least significant or most significant byte, respectively). The network
    // order is defined in terms of Big Endian format.
    
    if (inet_aton (dotted_form, &server_addr.sin_addr) == 0) {
        printf ("Malformed Dotted decimal address supplied\n");
        return -1;
    }

    // indicate what port number of server we want to connect to. 
    // We convert the value into network byte order. This time we use htons
    // since port_num is a "short". There is an equivalent "htonl" to handle
    // long values.
    server_addr.sin_port = htons (port_num);
    
    // (2c) Now actively establish connection
    /*
      The connect system call below takes the socket and the populated
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
    int conn_status;
    conn_status =
        connect (conn_sock,    // this was the connection socket handle we
                               // created earlier 
                 // the second arg is supposed to be of type "sockaddr *" which
                 // is a typedef declared in the OS headers to  represent a 
                 // generic network family. However, we  are using the IPv4
                 // family and hence had initialized the "sockaddr_in"
                 // structure. Therefore, now we must cast the sockaddr_in to
                 // sockaddr
                 (struct sockaddr *)&server_addr,
                 // indicate the size of the structure
                 sizeof (struct sockaddr));

    // error checking
    if (conn_status == -1 ) {
        perror ("connect failed");
        return -1;
    }

    printf ("Client: ESTABLISHED A CONNECTION\n");

    // send and receive some data for which we allocate buffers to hold the
    // information. 
    char send_buff[1024];
    char rcv_buff [1024];

    // Once again, zero out the buffers
    memset (send_buff, 0, sizeof (send_buff));
    memset (rcv_buff, 0, sizeof (rcv_buff));

    printf ("Enter some data to send to server (type exit to kill server): ");
    scanf ("%[^\n]s", send_buff);

    int send_status = 
        send (conn_sock, // I/O is done on this socket handle
              // second parameter is the buffer you send
              (void *) send_buff,
              // 3rd param is the length of the buffer
              sizeof (send_buff),
              // we ignore the flags argument
              0);
        
    // error checking
    if (send_status == -1) {
        perror ("send failed");
        return -1; // note we should really cleanup but we ignore it
    }

    // Now we expect the server to send us a reply. So we block until
    // we receive something
    int recv_status = recv (conn_sock,  // I/O done with new sock handle
                            // second parameter is a buffer into which you
                            // receive data  
                            //
                            // Linux needs a (void *) buffer
                            (void *) rcv_buff,
                            // third parameter is the size of the buffer
                            sizeof (rcv_buff),
                            // last parameter is a flag that we will ignore.
                            0);

    // network I/O is tricky. Sometime you do not receive all the
    // bytes you have requested. We are not going to check for this
    // condition. On Linux you can force the kernel to not return
    // until all the requested bytes are read by using a special
    // flag 
    if (recv_status == -1) {
        perror ("recv failed");
        return -1; // note we should really cleanup but we ignore it
    }

    printf ("Client received: %s\n", rcv_buff);

    // STEP 6: close the handle because we are done
    (void) close (conn_sock);

    return 0;
}
