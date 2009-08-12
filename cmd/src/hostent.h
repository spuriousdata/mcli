#ifndef __HOSTENT_H__
#define __HOSTENT_H__

#define MC_DEFAULT_PORT 11227
#define NUMRESP 10

struct __mchost {
	char *host;
	int port;
	struct __mchost *__next;
};

#endif
