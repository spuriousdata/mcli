#ifndef __RL_COMPLETE_H__
#define __RL_COMPLETE_H__



char **mci_completion(const char *text, int start, int end);
char **history_matches(const char *text, int start);
char *command_generator(const char *text, int state);
void initialize_readline();

char *commands[] = {
	"set",
	"add",
	"replace",
	"append",
	"prepend",
	"cas",
	"get",
	"gets",
	"delete",
	"incr",
	"decr",
	"stats",
	"flush_all",
	"version",
	"quit",
	"with",
	"list",
	NULL
};

#endif
