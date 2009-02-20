#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

extern int _conf_use_socks;
extern int _conf_socks_proto;
extern int _conf_socks_port;
extern char *_conf_socks_username;
extern char *_conf_socks_host;
extern char *_conf_socks_password;

int socksify(struct sockaddr_in *destaddr);
int socks5_connect(struct sockaddr_in *destaddr);

int socksify(struct sockaddr_in *destaddr)
{
	if (_conf_use_socks == 0) return 0; // don't use socks

	if (_conf_socks_proto == 5) return socks5_connect(destaddr);
	else {
		fprintf(stderr, "Invalid SOCKS protocol specified\n");
		return -1;
	}
}

int socks5_connect(struct sockaddr_in *destaddr)
{
	struct hostent *host;
	struct sockaddr_in sa;
	char sockspkt[514]; // version[1] nmethods[1] methods[2] username[255] password[255] 	
	char socksresp[BUFSIZ];
	char nmethods;
	int state, ulen, plen;
	int socks5_sockfd;

	if (_conf_socks_username != NULL) nmethods = 2;

	/* RFC 1928 SOCKS 5 */
	sockspkt[0] = 5; // socks 5
	sockspkt[1] = (_conf_socks_username ? 2 : 1); // if username is null number of methods is 1 (no auth) else 2 (auth or no auth)
	sockspkt[2] = 0; // no auth method
	sockspkt[3] = 2; // un/pw auth method

	socks5_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if ((host = gethostbyname(_conf_socks_host)) == 0) {
		fprintf(stderr, "Cannot find host\n");
		return -1;
	}

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(_conf_socks_port);

	memcpy(&(sa.sin_addr), host->h_addr, host->h_length);

	if ((state = connect(socks5_sockfd, (const struct sockaddr *)&sa, sizeof(sa))) < 0) {
		fprintf(stderr, "Cannot connect to server\n");
		return state;
	}
#ifdef DEBUG
	printf("Connected to socks proxy %s:%d\n", _conf_socks_host, _conf_socks_port);
#endif

	sendrecv(socks5_sockfd, &sockspkt, (_conf_socks_username ? 4 : 3), &socksresp, BUFSIZ);

	if (socksresp[0] != 5) {
		close(socks5_sockfd);
		return -1; // Protocol error -- server not socks5
	}

	if (socksresp[1] == (char)0xFF) {
		close(socks5_sockfd);
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
		
		sendrecv(socks5_sockfd, &sockspkt, (1 + 1 + ulen + 1 + plen), &socksresp, BUFSIZ);

		if (socksresp[0] != 1) {
			close(socks5_sockfd);
			return -1; // Protocol error -- subnegotiation not protocol 1
		}

		if (socksresp[1] != 0) {
			close(socks5_sockfd);
			fprintf(stderr, "Error connecting to socks proxy, bad username/password\n");
			return -1;
		}
	}

	/* either through no-authentication or successful authentication, we've arrived here
	 * we can now connect to the remote server */

	memset(sockspkt, 0, 514);
	sockspkt[0] = 5; // socks5
	sockspkt[1] = 1; // connect
	sockspkt[2] = 0; // reserved
	sockspkt[3] = 1; // ip4
	memcpy((sockspkt+4), &(destaddr->sin_addr.s_addr), 4);
	memcpy((sockspkt+8), &(destaddr->sin_port), 2);

	sendrecv(socks5_sockfd, &sockspkt, 10, &socksresp, BUFSIZ);

	if (socksresp[0] != 5) {
		close(socks5_sockfd);
		return -1; // Protocol error -- subnegotiation not protocol 1
	}

	if (socksresp[1] != 0) {
		close(socks5_sockfd);
		return -1; // connect didn't succeed
	}

	return socks5_sockfd;
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
