#ifndef __MCI_H__
#define __MCI_H__

extern struct __mchost *sl_head;
extern int _conf_max_connections;

extern FILE *yyin;
extern "C" {
	extern int yyparse();
	int yylen(void);
}

#ifdef DEBUG
extern int yydebug;
#endif

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
char *check_pipe(char *command);
void set_pipe_command(char *command);
int parseopts(int argc, char **argv);
void usage(char *);
int enbuffer(char **buffer, int *used, int *len, char *data, int data_len);
char *do_pipe_cmd(char *data, int *len);

#endif
