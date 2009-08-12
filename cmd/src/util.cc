#include <string.h>
#include <ctype.h>
#include "util.h"

char *rtrim(char *s)
{
	register char *end;
	register int len;

	len = strlen(s);
	while (*s && len) {
		end = s + (len-1);
		if (isspace(*end)) *end = 0;
		else break;
		len = strlen(s);
	}
	return s;
}

char *ltrim(char *s)
{
	register char *c;
	register int len;

	len = strlen(s);
	c = s;
	while (*c && len) {
		if (isspace(*c)) c++;
		else break;
	}

	/**
	 * copy the new string back to the old string and return it. We do this
	 * in case s was malloc'd -- this way it can be free'd
	 */
	memmove(s, c, len-strlen(c));
	return s;
}

char *trim(char *s)
{
	return ltrim(rtrim(s));
}
