#ifndef __HOSTENT_H__
#define __HOSTENT_H__

#include <string>

class HostEnt
{
	public:
		HostEnt(std::string host, unsigned short port) : host(host), port(port) {}
		std::string host;
		unsigned short port;
};


#endif
