#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rl_complete.h"

void initialize_readline()
{
	rl_readline_name = "mci";
	rl_attempted_completion_function = (CPPFunction*)mci_completion;
}

char **mci_completion(const char *text, int start, int end)
{
	char **matches;

	matches = (char **)NULL;

	/* if word is at start of line, then we should try to complete it from the command list */
	if (start == 0) matches = completion_matches(text, command_generator);
	else matches = history_matches(text, start);

	return matches;
}

char *command_generator(const char *text, int state)
{
	static int list_index, len;
	char *name;

	if (!state) {
		list_index = 0;
		len = strlen(text);
	}

	while (name = commands[list_index++]) {
		if (name == NULL) break;
		if (strncmp(name, text, len) == 0) return (strdup(name));
	}

	return (char *) NULL;
}

char **history_matches(const char *text, int start)
{
	static int len, i;
	HIST_ENTRY **h, *e;
	char *name;
	char **matches;

	/* Don't run this yet */
	return (char **)NULL;

	/* end */

	/*
	if ((h = history_list()) == NULL) return (char **)NULL;
	list_index = 0;
	len = strlen(text);
	
	for (i = 0, e = h[i]; e != NULL; i++) {
		if (e == NULL) return (char **)NULL;
		if (strncmp(((e->line)+start), text, len) == 0) 
	}

	return (char **)NULL;
	*/
}
