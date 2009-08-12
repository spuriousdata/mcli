#include <pcre.h>
#include <cstdio>
#include <cstring>
#include "pgrep.h"

/**
  * 'grep' for the /pattern/ return a pointer to a list of matched lines
  * if no matches or an error occurrs, return original string
  */
char *pgrep(char *pattern, char *subject, int *datalen)
{
	int rc, error_offset, patlen, num_matches, possible_matches, subject_len, ovector[9];
	const char *error;
	pcre *re;
	char *fullpattern;
	char *prefix = "^.*?";
	char *postfix = ".*?$";
	char **matches;
	const char *match;
	char *op;

	num_matches = 0;
	subject_len = *datalen;

	memset(ovector, 0, (9*sizeof(int)));

	/* since this is a grep operation the maximum number of matches is the number
	   of lines in the subject string */
	possible_matches = sizeof(char *) * get_count(subject, '\n');
	matches = (char **)malloc(possible_matches);
	memset(matches, 0, possible_matches);

	patlen = strlen(prefix) + strlen(postfix) + strlen(pattern) + 1;

	fullpattern = (char *)malloc(patlen);
	memset(fullpattern, 0, patlen);

	strcat(fullpattern, prefix);
	strcat(fullpattern, pattern);
	strcat(fullpattern, postfix);

	if ((re = pcre_compile(fullpattern, (PCRE_NEWLINE_CRLF | PCRE_MULTILINE), &error, &error_offset, NULL)) == NULL) {
		fprintf(stderr, "Regular expression compilation failed at offset %d: %s\n", error_offset, error);
		return subject;
	}

	op = subject;
	while ((rc = pcre_exec(re, NULL, op, subject_len, 0, 0, ovector, 9)) == 1) {
		pcre_get_substring(op, ovector, rc, 0, &match);
		matches[num_matches++] = (char *)match;
		op += ovector[1];
		subject_len -= ovector[1];

		if (subject_len == 0) break;
	}

	free(fullpattern);
	pcre_free(re);

	if (rc < 0 && num_matches == 0) {
		switch (rc) {
			case PCRE_ERROR_NOMATCH:
				return subject;
			default:
				fprintf(stderr, "Unknown PCRE error occurred. Error: %d\n", rc);
				return subject;
		}
	} else if (rc > 1) {
		fprintf(stderr, "Submatches '(...)' are not allowed in grep operation");
		return subject;
	}

	/* free subject and matches set subject to new concatenated match string */
	if (num_matches > 0) {
		free(subject);
		subject = match_cat(matches, possible_matches);
		*datalen = strlen(subject);
		free(matches);
	}

	return subject;
}

/* concatenation match strings and free them */
char *match_cat(char **matches, int max)
{
	char *answer, *match;
	int i, answer_len = 0;

	for (i = 0; i < max; i++) {
		if ((match = matches[i]) == NULL) break;
		answer_len += strlen(match)+1;
	}

	answer = (char *)malloc(answer_len + 1);
	memset(answer, 0, answer_len + 1);

	for (i = 0; i < max; i++) {
		if ((match = matches[i]) == NULL) break;
		strcat(answer, match);
		strcat(answer, "\n");
		pcre_free(match); // we can release this memory now
	}

	return answer;
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
