#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <string>
#include <sstream>
#include <exception>
#include <vector>

class Connection
{
	public:
		Connection(const char *hostname, unsigned short port); 
		~Connection() { if (sockfd) close(sockfd); }
		void send(std::string data);
		int recv(char *buf, size_t buflen);
		const int get_sockfd() const { return sockfd; }
		const std::string get_host() const
		{
			std::stringstream s;
			s << hostname << ":" << port;
			return s.str();
		}
	
	private:
		int sockfd;
		int flags;
		std::string hostname;
		unsigned short port;

		bool socksify();
		int open(const char *hostname, unsigned short port);
		int put_on_socks(const char *hostname, unsigned short port);
		int sendrecv(int sock, char *s, int slen, char *r, int rlen);
};

class ConnectionException : public std::exception
{
	public:
		ConnectionException(int code, std::string s) throw() : std::exception(), msg(s), code(code) {}
		~ConnectionException() throw() {}
		const char *what() const throw()
		{
			std::stringstream s;
			s << "Error(" << code << ") " << msg;
			return s.str().c_str();
		}

	private:
		std::string msg;
		int code;
};

class SocketSendException : public ConnectionException 
{
	public:
		SocketSendException(int code, std::string s) : ConnectionException(code, s) {}
};

class SocketRecvException : public ConnectionException
{
	public:
		SocketRecvException(int code, std::string s) : ConnectionException(code, s) {}
};

class ConnectionClosedException : public ConnectionException 
{
	public:
		ConnectionClosedException(int code, std::string s) : ConnectionException(code, s) {}
};

#endif
