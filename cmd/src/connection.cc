#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "connection.h"
#include "socks.h"

extern int i_verbose;

int open_connection(char *hostname, int port)
{
	struct hostent *host;
	struct sockaddr_in sa;
	int state,sockfd;

	if ((host = gethostbyname(hostname)) == 0) {
		fprintf(stderr, "Cannot find host: %s\n", hostname);
		return -1;
	}

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);

	memcpy(&(sa.sin_addr), host->h_addr, host->h_length);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if ((state = connect(sockfd, (const struct sockaddr *)&sa, sizeof(sa))) < 0) {
		fprintf(stderr, "Cannot connect to server\n");
		return state;
	}
	if (i_verbose) printf("Connected to %s:%d\n", hostname, port);
	return sockfd;
}
