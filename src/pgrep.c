#include <pcre.h>
#include <stdio.h>
#include <string.h>
#include "pgrep.h"

char *pgrep(char *pattern, char *subject, unsigned datalen)
{
	int rc, error_offset, patlen, oveclen, *ovector;
	const char *error;
	pcre *re;
	char *fullpattern;
	char *prefix = "^(.*?";
	char *postfix = ".*?)$";
	const char **matches;

	/**
	  * size of ovector should be 3 times (data is returned in triplets)
	  * the number of possible matches. I'm counting the newlines to determine
	  * the number of possible matches.
	  */
	oveclen = get_count(subject, '\n') * 3;
	ovector = (int *)malloc(sizeof(int) * oveclen);

	patlen = strlen(prefix) + strlen(postfix) + strlen(pattern) + 1;

	fullpattern = (char *)malloc(patlen);
	memset(fullpattern, 0, patlen);

	strcat(fullpattern, prefix);
	strcat(fullpattern, pattern);
	strcat(fullpattern, postfix);

	if ((re = pcre_compile(pattern, (PCRE_NEWLINE_CRLF | PCRE_MULTILINE), &error, &error_offset, NULL)) == NULL) {
		fprintf(stderr, "Regular expression compilation failed at offset %d: %s\n", error_offset, error);
		return subject;
	}

	if ((rc = pcre_exec(re, NULL, subject, datalen, 0, 0, ovector, oveclen)) < 0) {

		free(ovector);
		free(fullpattern);
		pcre_free(re);

		switch (rc) {
			case PCRE_ERROR_NOMATCH:
				return subject;
			default:
				fprintf(stderr, "Unknown PCRE error occurred. Error: %d\n", rc);
				return subject;
		}
	}

	pcre_get_substring_list(subject, ovector, rc, &matches);

	free(ovector);
	free(fullpattern);
	pcre_free(re);

	return subject;
}

/**
  * count the occurrances of character c in string s
  */
int get_count(char *s, char ch)
{
	int count = 0;
	char *c, *_s;

	_s = (char *)malloc(strlen(s)+1);
	strcpy(_s, s);

	while ((c = strrchr(_s, ch)) != NULL) {
		*c = 0;
		count++;
	}

	free(_s);
	return count;
}
