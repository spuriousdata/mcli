%{
// Parser
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>

//#include "hostent.h"
#include "Configuration.h"

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
					Configuration *mciconfig = Configuration::get_instance();
					mciconfig->set_value("use_socks", (bool)$2);
				}
				;

socks_directive: SOCKS_DNS NUMBER
				{
					Configuration *mciconfig = Configuration::get_instance();
					mciconfig->set_value("socks_dns", $2);
				}
				;

socks_directive: SOCKS_PROTO NUMBER
				{
					Configuration *mciconfig = Configuration::get_instance();
					mciconfig->set_value("socks_proto", $2);
				}
				;

socks_directive: SOCKS_PORT PORTNUM
				{
					Configuration *mciconfig = Configuration::get_instance();
					mciconfig->set_value("socks_port", $2);
				}
				;

socks_directive: SOCKS_HOST HOSTNAME
				{
					Configuration *mciconfig = Configuration::get_instance();
					mciconfig->set_value("socks_host", (char *)$2);
				}
				;

socks_directive: SOCKS_HOST IPADDR
				{
					Configuration *mciconfig = Configuration::get_instance();
					mciconfig->set_value("socks_host", (char *)$2);
				}
				;

socks_directive: SOCKS_USERNAME STRING
				{
					Configuration *mciconfig = Configuration::get_instance();
					mciconfig->set_value("socks_username", $2);
				}
				;

socks_directive: SOCKS_PASSWORD STRING
				{
					Configuration *mciconfig = Configuration::get_instance();
					mciconfig->set_value("socks_password", $2);
				}
				;

maxcon_directive: MAX_CONNECTIONS NUMBER
				{
					Configuration *mciconfig = Configuration::get_instance();
					mciconfig->set_value("max_connections", $2);
				}
				;

server_directive: SERVER HOSTNAME
				{
					Configuration *mciconfig = Configuration::get_instance();
					if (thost != NULL || tport != -1) {
						mciconfig->add_host(thost, Configuration::MC_DEFAULT_PORT);
						thost = NULL;
					}
					thost = $2;
				}
				;

server_directive: SERVER IPADDR
                {
					Configuration *mciconfig = Configuration::get_instance();
					if (thost != NULL || tport != -1) {
						mciconfig->add_host(thost, Configuration::MC_DEFAULT_PORT);
						thost = NULL;
					}
					thost = $2;
                }
                ;

port_directive: PORT PORTNUM
                {
					Configuration *mciconfig = Configuration::get_instance();
					if (thost == NULL) {
						fprintf(stderr, "Syntax error, port with no hostname\n");
					} else {
                 		tport = $2;
						mciconfig->add_host(thost, tport);
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
