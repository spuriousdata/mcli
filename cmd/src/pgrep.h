#ifndef __PGREP_H__
#define __PGREP_H__

#ifdef HAVE_LIBPCRE

char *pgrep(char *pattern, char *subject, int *datalen);
int get_count(char *s, char c);
char *match_cat(char **matches, int max);

#endif

#endif
