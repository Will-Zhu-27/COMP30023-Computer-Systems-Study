/* A simple server in the internet domain using TCP
The port number is passed as an argument


 To compile: gcc server.c -o server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>

char* toUppercase(char *string);
int checkEnd(char *string);

int main(int argc, char **argv)
{
	int sockfd, newsockfd, portno;// clilen;
	char buffer[256];
	char *replyConvertUppercase = NULL;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	int n;

	if (argc < 2)
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	 /* Create TCP socket */

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}


	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);

	/* Create address we're going to listen on (given port number)
	 - converted to network byte order & any IP address for
	 this machine */

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);  // store in machine-neutral format

	 /* Bind address to the socket */

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0)
	{
		perror("ERROR on binding");
		exit(1);
	}

	/* Listen on socket - means we're ready to accept connections -
	 incoming connection requests will be queued */

	listen(sockfd,5);

	clilen = sizeof(cli_addr);

	/* Accept a connection - block until a connection is ready to
	 be accepted. Get back a new file descriptor to communicate on. */

	newsockfd = accept(	sockfd, (struct sockaddr *) &cli_addr,
						&clilen);

	if (newsockfd < 0)
	{
		perror("ERROR on accept");
		exit(1);
	}

	bzero(buffer,256);

	while(!checkEnd(buffer)){
		/* Read characters from the connection,
		then process */
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);

		if (n < 0)
		{
			perror("ERROR reading from socket");
			exit(1);
		}

		printf("Here is the message: %s\n",buffer);
	
		replyConvertUppercase = toUppercase(buffer);
	
		n = write(newsockfd,replyConvertUppercase,18);
		free(replyConvertUppercase);
		
		if (n < 0)
		{
			perror("ERROR writing to socket");
			exit(1);
		}
	}
	/* close socket */

	close(sockfd);

	return 0;
}

char* toUppercase(char *string) {
	int i;
	int length = strlen(string);
	char *ret = (char *)calloc(length, sizeof(char));
	if (ret == NULL){
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < length; i++){
		ret[i] = toupper(string[i]);
	}
	return ret;
}

int checkEnd(char *string) {
    char temp[4];
    int i;
    for (i = 0; i < 4; i++){
        temp[i] = string[i];
    }

    if(strcmp(temp,"quit") == 0) {
        return 1;
    } else {
        return 0;
    }
}
