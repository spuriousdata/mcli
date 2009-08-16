#ifndef __HOSTENT_H__
#define __HOSTENT_H__


struct __mchost {
	char *host;
	int port;
	struct __mchost *__next;
};


#endif
