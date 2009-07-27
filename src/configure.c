#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mci.h"
#include "configure.h"

struct __mchost *sl_head = NULL;
struct __mchost *sl_tail = NULL;

int add_serverentry_str(char *serverstr)
{
	char *colon;
	int port;

	if ((colon = strchr(serverstr, ':')) == NULL){
		/* TODO: This should use the default port instead */
		fprintf(stderr, "invalid server format specified\n");
		return -1;
	}
	*colon = 0;
	sscanf((colon+1), "%d", &port);

	return add_serverentry(serverstr, port);
}

int add_serverentry(char *host, int port)
{
	struct __mchost *e;

	if ((e = (struct __mchost *)malloc(sizeof(struct __mchost))) == NULL) return -1;
	e->host = host;
	e->port = port;
	e->__next = NULL;

	if (sl_head == NULL) {
		sl_head = sl_tail = e;
	} else {
		sl_tail->__next = e;
		sl_tail = sl_tail->__next;
	}

	return 0;
}
