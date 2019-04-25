/* A simple client program for server.c

   To compile: gcc client.c -o client

   To run: start the server, then the client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int checkEnd(char *string);

int main(int argc, char **argv)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char readBuffer[256];
    char writeBuffer[256];

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    /* Translate host name into peer's IP address ;
     * This is name translation service by the operating system
     */
    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    /* Building data structures for socket */

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);

    /* Create TCP socket -- active open
    * Preliminary steps: Setup: creation of active open socket
    */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(0);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(0);
    }

    /* Do processing
    */
    bzero(readBuffer, 256);
    while (!checkEnd(readBuffer))
    {
        printf("Please enter the message: ");

        bzero(writeBuffer, 256);

        fgets(writeBuffer, 255, stdin);

        n = write(sockfd, writeBuffer, strlen(writeBuffer));

        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(0);
        }

        bzero(readBuffer, 256);

        n = read(sockfd, readBuffer, 255);

        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(0);
        }

        printf("%s\n", readBuffer);
    }

    printf("end connection\n");

    return 0;
}

int checkEnd(char *string) {
    char temp[4];
    int i;
    for (i = 0; i < 4; i++){
        temp[i] = string[i];
    }

    if(strcmp(temp,"QUIT") == 0) {
        return 1;
    } else {
        return 0;
    }
}
