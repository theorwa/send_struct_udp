#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

void DieWithError(char *err) {
   perror(err);
   exit(1);
}

    struct listen_package_t
{
    uint32_t magic;
    uint16_t port_number;
    char shell_command[256];
  };

int main(int argc, char *argv[])
{
     int sock;                        /* Socket */
     struct sockaddr_in echoServAddr; /* Local address */
     struct sockaddr_in echoClntAddr; /* Client address */
     unsigned int cliAddrLen;         /* Length of incoming message */
     unsigned short echoServPort;     /* Server port */
     int recvMsgSize;                 /* Size of received message */ 

  struct listen_package_t * temp = malloc(sizeof(struct listen_package_t));

  if (!argv[1]) {
      fprintf(stderr,"no port number provided");
     exit(1);
  }

      echoServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for sending/receiving datagrams */  
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    DieWithError("socket() failed");

  /* Construct local address structure */
  memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
  echoServAddr.sin_family = AF_INET;                /* Internet address family */
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
  echoServAddr.sin_port = htons(echoServPort);      /* Local port */

  /* Bind to the local address */ 
  if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
     DieWithError("bind() failed");

  for (;;) /* Run forever */
  {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(echoClntAddr);

        /* Block until receive message from  a client */
        if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
           DieWithError("recvfrom() failed");

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        printf("Incoming Length: %u\n", cliAddrLen);
        printf("Received: %zu\n", temp->magic);   /* Print the string in sent struct */

   }
      /* NOT REACHED */

        close(sock);
        return 0;
  } 