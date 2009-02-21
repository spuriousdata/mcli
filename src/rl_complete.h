#ifndef __RL_COMPLETE_H__
#define __RL_COMPLETE_H__

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
