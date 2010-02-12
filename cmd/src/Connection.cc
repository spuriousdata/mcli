#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cstring>
#include <algorithm>        // fill_n
#include <sys/socket.h>     // socket(), send(), recv(), connect()
#include <sys/types.h>      // socklen_t (used by connect())
#include <netinet/in.h>     // sockaddr_in
#include <arpa/inet.h>      // htons()
#include <netdb.h>          // gethostbyname(), struct hostent

#include "Configuration.h"
#include "Connection.h"

Connection::Connection(const char *hostname, unsigned short port) :
	hostname(hostname), port(port)
{
	if (socksify()) sockfd = put_on_socks(hostname, port);
	else sockfd = open(hostname, port);
}

int Connection::open(const char *hostname, unsigned short port)
{
	struct hostent *host;
	struct sockaddr_in sa;
	int sockfd, state;

	if ((host = gethostbyname(hostname)) == 0) {
		throw ConnectionException(-1, std::string("Cannot find host: ").append(hostname));
	}

	/*
	 * should this be
	 *   fill_n(&sa, 1, 0);
	 * or
	 *   fill_n(&sa, sizeof(sa), 0);
	 */
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);

	memcpy(&(sa.sin_addr), host->h_addr, host->h_length);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if ((state = connect(sockfd, (const struct sockaddr *)&sa, sizeof(sa))) < 0) 
		throw ConnectionException(state, "Cannot connect to server");

	if (Configuration::get_instance()->get_as_bool("verbose"))
		std::cout << "Connected to " << hostname << ":" << port << std::endl;

	socksify();

	return sockfd;
}

bool Connection::socksify()
{
	Configuration *conf = Configuration::get_instance();
	
	if (conf->get_as_bool("use_socks")) {
		if (conf->get_as_int("socks_proto") == 5 || conf->get_value("socks_proto") == "") {
			return true;
		} else if (conf->get_as_int("socks_proto") == 4) {
			throw std::invalid_argument("SOCKS protocl 4 is not currently supported");
		} else {
			throw std::invalid_argument("Unknown SOCKS protocol");
		}
	}
	return false;
}

int Connection::put_on_socks(const char *hostname, unsigned short port)
{
	Configuration *mciconfig = Configuration::get_instance();
	char sockspkt[514]; // version[1] nmethods[1] methods[2] username[255] password[255] 	
	char socksresp[BUFSIZ];
	char nmethods;
	int state, ulen, plen;
	int sockfd;
	struct hostent *host;
	short netorder_port;

	if (!mciconfig->get_value("socks_username").empty()) nmethods = 2;

	/* RFC 1928 SOCKS 5 */
	sockspkt[0] = 5; // socks 5
	sockspkt[1] = (mciconfig->get_value("socks_username").empty() ? 1 : 2); /*
																			 * if username is null number of methods is 
																			 * 1 (no auth) else 
																			 * 2 (auth or no auth)
																			 */
	sockspkt[2] = 0; // no auth method
	sockspkt[3] = 2; // un/pw auth method

	if ((sockfd = open(mciconfig->get_value("socks_host").c_str(), mciconfig->get_as_int("socks_port"))) == -1)
		std::cerr << "Error connecting to socks proxy";

	if (mciconfig->get_as_bool("verbose")) 
		std::cout << "Connected to socks proxy " << mciconfig->get_value("socks_host") << ":" << mciconfig->get_as_int("socks_port");

	sendrecv(sockfd, sockspkt, (mciconfig->get_value("socks_username").empty() ? 3 : 4), socksresp, BUFSIZ);

	if (socksresp[0] != 5) 
		throw ConnectionException(-1, "SOCKS server does not suport protocol 5");

	if (socksresp[1] == (char)0xFF)
		throw ConnectionException(-1, "SOCKS server does not support any of the proposed authentication methods");

	memset(sockspkt, 0, 514);

	/* RFC 1929 Username/Password Authentication for SOCKS V5 */
	if (socksresp[1] == 2) {
		// un/pw required
		sockspkt[0] = 1; // un/pw subnegotiation ptorocol 1

		ulen = mciconfig->get_value("socks_username").length();
		sockspkt[1] = ulen;
		memcpy((sockspkt+2), mciconfig->get_value("socks_username").c_str(), ulen); // copy in the username

		plen = mciconfig->get_value("socks_password").length();
		sockspkt[2+ulen] = plen;
		memcpy((sockspkt+2+ulen+1), mciconfig->get_value("socks_password").c_str(), plen); // copy in the password
		
		sendrecv(sockfd, sockspkt, (1 + 1 + ulen + 1 + plen), socksresp, BUFSIZ);

		if (socksresp[0] != 1)
			throw ConnectionException(-1, "Unknown error, SOCKS subnegotiation protocol is not 1");

		if (socksresp[1] != 0) 
			throw ConnectionException(-1, "Error connecting to socks proxy, bad username/password");
	}

	/* either through no-authentication or successful authentication, we've arrived here
	 * we can now connect to the remote server */

	netorder_port = htons(port);

	memset(sockspkt, 0, 514);
	sockspkt[0] = 5; // socks5
	sockspkt[1] = 1; // connect
	sockspkt[2] = 0; // reserved
	if (mciconfig->get_as_bool("socks_dns")) {
		sockspkt[3] = 3;
		plen = strlen(hostname);
		sockspkt[4] = plen & 0xFF; // hostname should be shorter than 254
		memcpy((sockspkt+5), hostname, plen);
		memcpy((sockspkt+5+plen), &netorder_port, 2);
		plen += 5 + 2;
	} else {
		sockspkt[3] = 1; // ip4
		host = gethostbyname(hostname);
		if (host->h_length != 4) 
			std::cerr << "Something bad happened -- host->h_length is not 4";
		memcpy((sockspkt+4), &(host->h_addr), 4);
		memcpy((sockspkt+8), &netorder_port, 2);
		plen = 10;
	}

	sendrecv(sockfd, sockspkt, plen, socksresp, BUFSIZ);

	if (socksresp[0] != 5)
		throw ConnectionException(-1, "SOCKS server does not suport protocol 5");

	if (socksresp[1] != 0)
		throw ConnectionException(-1, "Unknown SOCKS error, connection to endpoint failed");

	return sockfd;
}

void Connection::send(std::string data)
{
	std::stringstream s;
	if (::send(sockfd, data.c_str(), data.length(), 0) == -1) {
		s << "Error sending data to socket for host " << hostname << " port " << port;
		throw SocketSendException(-1, s.str());
	}
}

int Connection::recv(char *buf, size_t buflen)
{
	int numbytes;
	std::stringstream s;
	
	std::fill_n(buf, buflen, 0);
	if ((numbytes = ::recv(sockfd, buf, buflen, 0)) == -1) {
		s << "Error reading from socket for host " << hostname << " port " << port;
		throw SocketRecvException(-1, s.str());
	}

	return numbytes;
}

int Connection::sendrecv(int sock, char *s, int slen, char *r, int rlen)
{
	int numbytes;

	if (::send(sock, s, slen, 0) == -1) {
		fprintf(stderr, "Error sending data to server\n");
		return -1;
	}

	memset(r, 0, rlen); // clean out
	if ((numbytes = ::recv(sock, r, rlen, 0)) == -1) {
		fprintf(stderr, "Error reading from server\n");
		return -1;
	}

	return numbytes;
}
