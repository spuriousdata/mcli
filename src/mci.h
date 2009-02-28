#ifndef __MCI_H__
#define __MCI_H__

#define MC_DEFAULT_PORT 11227
#define NUMRESP 10

struct __mchost {
	char *host;
	int port;
	struct __mchost *__next;
};

extern struct __mchost *sl_head;
extern int _conf_max_connections;

extern FILE *yyin;
extern int yyparse();
#ifdef DEBUG
extern int yydebug;
#endif
extern void initialize_readline();

extern char *optarg;
extern int optind;
extern int errno;

int internal_command(char *);
int check_end_mc_response(char *);
char *get_pager(void);
int initialize(void);
int configure(void);
int connect_serverlist(void);
int cleanup(void);
char *get_active_servername(void);
char *get_servername(int);
int communicate(char *);
char *check_set(char *command);
int parseopts(int argc, char **argv);
void usage(char *);

#endif
