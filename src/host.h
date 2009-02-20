#define MC_DEFAULT_PORT 11227

struct __mchost {
	char *host;
	int port;
	struct __mchost *__next;
};

