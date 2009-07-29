#include <pcre.h>
#include <stdio.h>

char *pgrep(char *pattern, char *subject, unsigned datalen)
{
	int rc, error_offset, ovector[30];
	const char *error;
	pcre *re;

	if ((re = pcre_compile(pattern, 0, &error, &error_offset, NULL)) == NULL) {
		fprintf(stderr, "Regular expression compilation failed at offset %d: %s\n", error_offset, error);
		return subject;
	}

	if ((rc = pcre_exec(re, NULL, subject, datalen, 0, 0, ovector, 30)) < 0) {
		switch (rc) {
			case PCRE_ERROR_NOMATCH:
				return NULL;
			default:
				fprintf(stderr, "Unknown PCRE error occurred. Error: %d\n", rc);
				return NULL;
		}
	}
}
