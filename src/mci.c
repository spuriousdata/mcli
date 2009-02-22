#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "mci.h"
#include "connection.h"

int i_verbose = 0;
int *socks, num_connections = 0;
int with_server = -1;
char *responses[] = {
	"ERROR\r\n",
	"CLIENT_ERROR",
	"SERVER_ERROR",
	"END\r\n",
	"OK\r\n",
	"DELETED\r\n",
	"STORED\r\n",
	"NOT_STORED\r\n",
	"EXISTS\r\n",
	"NOT_FOUND\r\n",
	NULL
};

int main(int argc, char **argv)
{
	int port;
	char *msg, *command = (char *)NULL;

	if (initialize() == -1) return -1;

	//resize_handler(SIGWINCH); // set up resize_signal_handler
	get_pager();

	initialize_readline();

	while (1) {

		if (command) {
			free(command);
			command = (char *)NULL;
		}

		command = readline("memcache> ");
		if (internal_command(command)) continue;

		if (command && *command) add_history(command);
		else continue;
		msg = (char *)malloc(strlen(command) + 2);
		memcpy(msg, command, strlen(command));
		strcat(msg, "\r\n");
		communicate(msg);
		free(msg);
	}

	if (cleanup() != 0) return -1;
	return 0;
}

int communicate(char *msg)
{
	int i, numbytes;
	char buf[BUFSIZ];

	if (with_server == -1 || (strncmp(msg, "get", 3) == 0)) {
		for (i = 0; i < num_connections; i++) {
			if (send(socks[i], msg, strlen(msg), 0) == -1) {
				fprintf(stderr, "Error sending data to server\n");
				return -1;
			}
	
			if (i_verbose) printf("Response from %d: %s\n", i, get_servername(i));
			
			do {
				memset(&buf, 0, BUFSIZ);
				if ((numbytes = recv(socks[i], &buf, BUFSIZ, 0)) == -1) {
					fprintf(stderr, "Error reading from server\n");
					return -1;
				}
				write(fileno(stdout), buf, numbytes);
			} while (check_end_mc_response(buf) == 0);
		}
	} else {
		if (send(socks[with_server], msg, strlen(msg), 0) == -1) {
			fprintf(stderr, "Error sending data to server\n");
			return -1;
		}
	
		do {
			memset(&buf, 0, BUFSIZ);
			if ((numbytes = recv(socks[with_server], &buf, BUFSIZ, 0)) == -1) {
				fprintf(stderr, "Error reading from server\n");
				return -1;
			}
			write(fileno(stdout), buf, numbytes);
		} while (check_end_mc_response(buf) == 0);
	}

	return 0;
}

int internal_command(char *s)
{
	char *t;
	int i;

	if (strncmp(s, "quit", 4) == 0) {
		exit(0);
	} else if (strncmp(s, "help", 4) == 0) {
		printf("list:          list connected servers\n");
		printf("help:          this help\n");
		printf("with <number>: bind commands to single server <number>\n");
		printf(" note that with will never bind 'get' or 'gets' calls\n");
		return 1;
	} else if (strncmp(s, "with", 4) == 0) {
		sscanf(s, "with %d", &with_server);	
		t = get_active_servername();
		printf("active server is now %d: %s\n", with_server, t);
		free(t);
		return 1;
	} else if (strncmp(s, "list", 4) == 0) {
		for (i = 0; i < num_connections; i++) {
			t = get_servername(i);
			printf(" %d: %s\n", i, t);
			free(t);
		}
		return 1;
	} else if (strncmp(s, "verbose", 7) == 0) {
		if (i_verbose == 0) i_verbose = 1;
		else i_verbose = 0;
		printf("verbose: %s\n", ((i_verbose) ? "on" : "off"));
		return 1;
	}
	return 0;
}

int check_end_mc_response(char *buf)
{
	int i = 0;
	char *response;

	while ((response = responses[i++]) != NULL) {
		if (strstr(buf, response) != NULL) return 1;
	}

	return 0;
}

char *get_pager(void)
{
	char *pager;

	if ((pager = getenv("PAGER")) != NULL) {
		return pager;
	} else {
		return "more";
	}
}

int initialize(void)
{
	if (configure() != 0) return -1;
	if (connect_serverlist() != 0) return -1;
	return 0;
}

int configure(void)
{
#ifdef DEBUG
	yydebug = 1;
#endif
	yyin = fopen("./mci.conf", "r");
	yyparse();

	if ((socks = (int *)malloc(sizeof(int) * _conf_max_connections)) == NULL) return -ENOMEM;
	return 0;
}

int connect_serverlist(void)
{
	struct __mchost *e;
	int sockfd;

	e = head;

	do {
		if ((sockfd = socksify(e->host, e->port)) == -1) {
			fprintf(stderr, "Error with socks connection\n");
			return -1;
		} else if (sockfd == 0) {
			// don't use socks
			if ((socks[num_connections++] = open_connection(e->host, e->port)) == -1) {
				fprintf(stderr, "Error connecting to socket\n");
				return -1;
			}
		} else {
			// use socks
			socks[num_connections++] = sockfd;
		}
	} while ((e = e->__next) != NULL);
	return 0;
}

int cleanup(void)
{
	struct __mchost *e, *f;
	int i;

	for (i = 0; i < num_connections; i++) {
		close(socks[i]);
	}

	e = head;
	do {
		f = e->__next;
		free(e->host);
		free(e);
	} while ((e = f) != NULL);

	return 0;
}

/* returns pointer to newly allocated string -- caller must free */
char *get_active_servername(void)
{
	return get_servername(with_server);
}

/* returns pointer to newly allocated string -- caller must free */
char *get_servername(int snum)
{
	struct __mchost *e;
	int len, i = 0;
	char *server;

	if (snum < 0) {
		asprintf(&server, "All");
		return server;
	}

	e = head;

	while (i++ != snum) {
		e = e->__next;
	}
	
	asprintf(&server, "%s:%d", e->host, e->port);

	return server;
}
