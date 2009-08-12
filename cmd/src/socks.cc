#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> //for close()

#include "socks.h"
#include "connection.h"

extern int i_verbose;

int socksify(char *hostname, int port)
{
	if (_conf_use_socks == 0) return 0; // don't use socks

	if (_conf_socks_proto == 5) return socks5_connect(hostname, port);
	else {
		fprintf(stderr, "Invalid SOCKS protocol specified\n");
		return -1;
	}
}

int socks5_connect(char *hostname, int port)
{
	char sockspkt[514]; // version[1] nmethods[1] methods[2] username[255] password[255] 	
	char socksresp[BUFSIZ];
	char nmethods;
	int state, ulen, plen;
	int sockfd;
	struct hostent *host;
	short netorder_port;

	if (_conf_socks_username != NULL) nmethods = 2;

	/* RFC 1928 SOCKS 5 */
	sockspkt[0] = 5; // socks 5
	sockspkt[1] = (_conf_socks_username ? 2 : 1); // if username is null number of methods is 1 (no auth) else 2 (auth or no auth)
	sockspkt[2] = 0; // no auth method
	sockspkt[3] = 2; // un/pw auth method

	if ((sockfd = open_connection(_conf_socks_host, _conf_socks_port)) == -1) {
		fprintf(stderr, "Error connecting to socks proxy\n");
		return sockfd;
	}

	if (i_verbose) printf("Connected to socks proxy %s:%d\n", _conf_socks_host, _conf_socks_port);

	sendrecv(sockfd, sockspkt, (_conf_socks_username ? 4 : 3), socksresp, BUFSIZ);

	if (socksresp[0] != 5) {
		close(sockfd);
		return -1; // Protocol error -- server not socks5
	}

	if (socksresp[1] == (char)0xFF) {
		close(sockfd);
		return -1; // No acceptable methods
	}

	memset(sockspkt, 0, 514);

	/* RFC 1929 Username/Password Authentication for SOCKS V5 */
	if (socksresp[1] == 2) {
		// un/pw required
		sockspkt[0] = 1; // un/pw subnegotiation ptorocol 1

		ulen = strlen(_conf_socks_username);
		sockspkt[1] = ulen;
		memcpy((sockspkt+2), _conf_socks_username, ulen); // copy in the username

		plen = strlen(_conf_socks_password);
		sockspkt[2+ulen] = plen;
		memcpy((sockspkt+2+ulen+1), _conf_socks_password, plen); // copy in the password
		
		sendrecv(sockfd, sockspkt, (1 + 1 + ulen + 1 + plen), socksresp, BUFSIZ);

		if (socksresp[0] != 1) {
			close(sockfd);
			return -1; // Protocol error -- subnegotiation not protocol 1
		}

		if (socksresp[1] != 0) {
			close(sockfd);
			fprintf(stderr, "Error connecting to socks proxy, bad username/password\n");
			return -1;
		}
	}

	/* either through no-authentication or successful authentication, we've arrived here
	 * we can now connect to the remote server */

	netorder_port = htons(port);

	memset(sockspkt, 0, 514);
	sockspkt[0] = 5; // socks5
	sockspkt[1] = 1; // connect
	sockspkt[2] = 0; // reserved
	if (_conf_socks_dns) {
		sockspkt[3] = 3;
		plen = strlen(hostname);
		sockspkt[4] = plen & 0xFF; // hostname should be shorter than 254
		memcpy((sockspkt+5), hostname, plen);
		memcpy((sockspkt+5+plen), &netorder_port, 2);
		plen += 5 + 2;
	} else {
		sockspkt[3] = 1; // ip4
		host = gethostbyname(hostname);
		if (host->h_length != 4) {
			close(sockfd);
			fprintf(stderr, "Something bad happened -- host->h_length is not 4\n");
			return -1;
		}
		memcpy((sockspkt+4), &(host->h_addr), 4);
		memcpy((sockspkt+8), &netorder_port, 2);
		plen = 10;
	}

	sendrecv(sockfd, sockspkt, plen, socksresp, BUFSIZ);

	if (socksresp[0] != 5) {
		close(sockfd);
		return -1; // Protocol error -- subnegotiation not protocol 1
	}

	if (socksresp[1] != 0) {
		close(sockfd);
		return -1; // connect didn't succeed
	}

	return sockfd;
}

int sendrecv(int sock, char *s, int slen, char *r, int rlen)
{
	int numbytes;

	if (send(sock, s, slen, 0) == -1) {
		fprintf(stderr, "Error sending data to server\n");
		return -1;
	}

	memset(r, 0, rlen); // clean out
	if ((numbytes = recv(sock, r, rlen, 0)) == -1) {
		fprintf(stderr, "Error reading from server\n");
		return -1;
	}

	return numbytes;
}
