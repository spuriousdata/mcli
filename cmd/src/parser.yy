%{
// Parser
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <arpa/inet.h>

#include "hostent.h"
#include "configure.h"
#include "Configuration.h"

extern Configuration mciconfig;
extern struct __mchost *sl_head;
extern struct __mchost *sl_tail;

extern "C" {
	int yylex(void);
	int yyerror(char *s);
	int yyparse(void);
}

char *thost = NULL;
int tport = -1;

%}

%token SERVER PORT MAX_CONNECTIONS USE_SOCKS SOCKS_PROTO SOCKS_PORT SOCKS_HOST SOCKS_USERNAME SOCKS_PASSWORD SOCKS_DNS

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
					mciconfig.set_value("use_socks", $2);
				}
				;

socks_directive: SOCKS_DNS NUMBER
				{
					mciconfig.set_value("socks_dns", $2);
				}
				;

socks_directive: SOCKS_PROTO NUMBER
				{
					mciconfig.set_value("socks_proto", $2);
				}
				;

socks_directive: SOCKS_PORT PORTNUM
				{
					mciconfig.set_value("socks_port", $2);
				}
				;

socks_directive: SOCKS_HOST HOSTNAME
				{
					mciconfig.set_value("socks_host", $2);
				}
				;

socks_directive: SOCKS_HOST IPADDR
				{
					mciconfig.set_value("socks_host", $2);
				}
				;

socks_directive: SOCKS_USERNAME STRING
				{
					mciconfig.set_value("socks_username", $2);
				}
				;

socks_directive: SOCKS_PASSWORD STRING
				{
					mciconfig.set_value("socks_password", $2);
				}
				;

maxcon_directive: MAX_CONNECTIONS NUMBER
				{
					mciconfig.set_value("max_connections", $2);
				}
				;

server_directive: SERVER HOSTNAME
				{
					if (thost != NULL || tport != -1) {
						add_serverentry(thost, Configuration::MC_DEFAULT_PORT);
						thost = NULL;
					}
					thost = $2;
				}
				;

server_directive: SERVER IPADDR
                {
					if (thost != NULL || tport != -1) {
						add_serverentry(thost, Configuration::MC_DEFAULT_PORT);
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

int yyerror(char *s)
{
    fprintf(stderr, "error: %s\n", s);
}
