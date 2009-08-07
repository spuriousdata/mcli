#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rl_complete.h"

#include "config.h"

#ifdef HAVE_LIBREADLINE
#  if defined(HAVE_READLINE_READLINE_H)
#    include <readline/readline.h>
#  elif defined(HAVE_READLINE_H)
#    include <readline.h>
#  else /* !defined(HAVE_READLINE_H) */
	 extern char *readline ();
#  endif /* !defined(HAVE_READLINE_H) */
	 char *cmdline = NULL;
#else /* !defined(HAVE_READLINE_READLINE_H) */
  /* no readline */
#endif /* HAVE_LIBREADLINE */

#ifdef HAVE_READLINE_HISTORY
#  if defined(HAVE_READLINE_HISTORY_H)
#    include <readline/history.h>
#  elif defined(HAVE_HISTORY_H)
#    include <history.h>
#  else /* !defined(HAVE_HISTORY_H) */

   	 extern void add_history ();
/*	 extern int write_history ();
	 extern int read_history (); */ // we're not using these //

#  endif /* defined(HAVE_READLINE_HISTORY_H) */
  /* no history */
#endif /* HAVE_READLINE_HISTORY */


#if defined(READLINE_4_DOT_2_PLUS) || (RL_VERSION_MAJOR >= 5) || (RL_VERSION_MAJOR == 4 && RL_VERSION_MINOR >= 2)
#	define completion_matches rl_completion_matches
#endif

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
