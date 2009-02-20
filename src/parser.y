%{
// Parser
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#include "host.h"

struct __mchost *head = NULL;
struct __mchost *tail = NULL;
int _conf_max_connections = 10;
int _conf_use_socks = 0;
int _conf_socks_proto = 5;
int _conf_socks_port = 1080;
char *_conf_socks_host = NULL;
char *_conf_socks_username = NULL;
char *_conf_socks_password = NULL;

char *thost = NULL;
int tport = -1;

int add_serverentry(char *, int);
%}

%token SERVER PORT MAX_CONNECTIONS USE_SOCKS SOCKS_PROTO SOCKS_PORT SOCKS_HOST SOCKS_USERNAME SOCKS_PASSWORD

%union {
	char *string;
	unsigned int number;
}

%token <string> IPADDR
%token <string> HOSTNAME
%token <number> PORTNUM
%token <string> STRING
%token <number> NUMBER

%%

//start: directives

directives:
			| directives directive
			;

directive: port_directive
			| server_directive
			| maxcon_directive
			| socks_directive
			;

socks_directive: USE_SOCKS NUMBER
				{
					_conf_use_socks = $2;
				}
				;

socks_directive: SOCKS_PROTO NUMBER
				{
					_conf_socks_proto = $2;
				}
				;

socks_directive: SOCKS_PORT PORTNUM
				{
					_conf_socks_port = $2;
				}
				;

socks_directive: SOCKS_HOST HOSTNAME
				{
					_conf_socks_host = $2;
				}
				;

socks_directive: SOCKS_HOST IPADDR
				{
					_conf_socks_host = $2;
				}
				;

socks_directive: SOCKS_USERNAME STRING
				{
					_conf_socks_username = $2;
				}
				;

socks_directive: SOCKS_PASSWORD STRING
				{
					_conf_socks_password = $2;
				}
				;

maxcon_directive: MAX_CONNECTIONS NUMBER
				{
					_conf_max_connections = $2;
				}
				;

server_directive: SERVER HOSTNAME
				{
					if (thost != NULL || tport != -1) {
						add_serverentry(thost, MC_DEFAULT_PORT);
						thost = NULL;
					}
					thost = $2;
				}
				;

server_directive: SERVER IPADDR
                {
					if (thost != NULL || tport != -1) {
						add_serverentry(thost, MC_DEFAULT_PORT);
						thost = NULL;
					}
					thost = $2;
                }
                ;

port_directive: PORT PORTNUM
                {
					if (thost == NULL) {
						fprintf(stderr, "Syntax error, port with no hostname\n");
					} else {
                 		tport = $2;
						add_serverentry(thost, tport);
						thost = NULL;
						tport = -1;
					}
                }
                ;

%%

int add_serverentry(char *host, int port)
{
	struct __mchost *e;

	if ((e = (struct __mchost *)malloc(sizeof(struct __mchost))) == NULL) return -1;
	e->host = host;
	e->port = port;
	e->__next = NULL;

	if (head == NULL) {
		head = tail = e;
	} else {
		tail->__next = e;
		tail = tail->__next;
	}

	return 0;
}

int yyerror(char *s)
{
    fprintf(stderr, "error: %s\n", s);
}

int yywrap()
{
    return;
}
