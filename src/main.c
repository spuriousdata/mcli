#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <errno.h>
#include "host.h"

#define NUMRESP 10

char *responses[NUMRESP] = {
	"ERROR\r\n",
	"CLIENT_ERROR",
	"SERVER_ERROR",
	"END\r\n",
	"OK\r\n",
	"DELETED\r\n",
	"STORED\r\n",
	"NOT_STORED\r\n",
	"EXISTS\r\n",
	"NOT_FOUND\r\n"
};

int *socks, num_connections = 0;
int with_server = -1;

extern struct __mchost *head;
extern int _conf_max_connections;

extern FILE *yyin;
extern int yyparse();
#ifdef DEBUG
extern int yydebug;
#endif
extern void initialize_readline();
//extern void resize_handler(int);

int internal_command(char *);
int check_end_mc_response(char *);
char *get_pager(void);
int initialize(void);
int configure(void);
int do_connect(void);
int cleanup(void);
char *get_active_servername(void);
char *get_servername(int);

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
#ifdef DEBUG
		printf("Sending(%s)\n", command);
#else
		msg = (char *)malloc(strlen(command) + 2);
		memcpy(msg, command, strlen(command));
		strcat(msg, "\r\n");
		communicate(msg);
		free(msg);
#endif		
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
		//print help
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
	}
	return 0;
}

int check_end_mc_response(char *buf)
{
	int i;
	for (i = 0; i < NUMRESP; i++) {
		if (strstr(buf, responses[i]) != NULL) return 1;
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
	if (do_connect() != 0) return -1;
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

int do_connect(void)
{
	struct __mchost *e;
	struct hostent *host;
	struct sockaddr_in sa;
	int state,sockfd;

	e = head;

	do {
		if ((host = gethostbyname(e->host)) == 0) {
			fprintf(stderr, "Cannot find host: %s\n", e->host);
			return -1;
		}

		memset(&sa, 0, sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_port = htons(e->port);

		memcpy(&(sa.sin_addr), host->h_addr, host->h_length);

		if ((sockfd = socksify(&sa)) == -1) {
			fprintf(stderr, "Error with socks connection\n");
			return -1;
		} else if (sockfd == 0) {
			// barefoot
			socks[num_connections] = socket(AF_INET, SOCK_STREAM, 0);
			if ((state = connect(socks[num_connections++], (const struct sockaddr *)&sa, sizeof(sa))) < 0) {
				fprintf(stderr, "Cannot connect to server\n");
				return state;
			}
			printf("Connected to %s:%d\n", e->host, e->port);
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

char *get_active_servername(void)
{
	return get_servername(with_server);
}

char *get_servername(int snum)
{
	struct __mchost *e;
	int len, i = 0;
	char *port, *server;

	if (snum < 0) {
		asprintf(&server, "All");
		return server;
	}

	e = head;

	while (i++ != snum) {
		e = e->__next;
	}
	
	asprintf(&port, "%d", e->port);
	/*       host           :   port          \0 */
	len = strlen(e->host) + 1 + strlen(port) + 1;

	server = (char *)malloc(len);
	memset(server, 0, len);
	strcat(server, e->host);
	strcat(server, ":");
	strcat(server, port);

	free(port);

	return server;
}
