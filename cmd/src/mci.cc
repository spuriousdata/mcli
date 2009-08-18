#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cctype>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "pgrep.h"
#include "mci.h"
#include "util.h"
#include "rl_complete.h"
#include "connection.h"
#include "socks.h"
#include "Configuration.h"
#include "HostEnt.h"

int i_verbose = 0;
int *socks, num_connections = 0;
int with_server = -1;
char *conf_file = NULL;
int verbose = 0;
typedef enum {
	NONE,
	GREP,
	SORT
} pipe_cmd_t;

struct __pipe_command {
	pipe_cmd_t type;
	char *args;
} pipe_command = { NONE, NULL };

char *responses[] = {
	"ERROR\r\n",
	"CLIENT_ERROR",
	"SERVER_ERROR",
	"END\r\n",
	"OK\r\n",
	"DELETED\r\n",
	"STORED\r\n",
	"NOT_STORED\r\n",
	"EXISTS\r\n",
	"NOT_FOUND\r\n",
	NULL
};

int main(int argc, char **argv)
{
	int port;
	char *msg, *command = (char *)NULL;

	if (parseopts(argc, argv) != 0) return -1;

	if (initialize() == -1) return -1;

	initialize_readline();

	while (1) {

		if (command) {
			free(command);
			command = (char *)NULL;
		}

		command = readline("memcache> ");
		if (internal_command(command)) continue;
		if ((command = check_set(command)) == NULL) continue;
		command = check_pipe(command);

		if (command && *command) add_history(command);
		else continue;
		msg = (char *)malloc(strlen(command) + 2);
		memset(msg, 0, strlen(command)+2);
		memcpy(msg, command, strlen(command));
		strcat(msg, "\r\n");
		communicate(msg);
		free(msg);
	}

	if (cleanup() != 0) return -1;
	return 0;
}

/* if comand is a set command, return a newly allocated string containing whole set command
 * else return original command
 */
char *check_set(char *command)
{
	char *setcmd, *tmp;
	int j1, j2, datalen;

	if (strncmp(command, "set", 3) != 0 ) return command;
	else {
		fprintf(stdout, "'set' not currently implemented\n");
		return NULL;
	}

	sscanf(command, "set %s %d %d %d", tmp, &j1, &j2, &datalen); // j1 & j2 are junk and we don't care what they are

	if ((setcmd = (char *)malloc(strlen(command) + 3)) == NULL) {
		fprintf(stderr, "Error allocating memory\n");
		exit(-1);
	}
	memset(setcmd, 0, strlen(command) + 3);
	memcpy(setcmd, command, strlen(command));
	strcat(setcmd, "\r\n");

	free(command);

	fputs("set> ", stdout);
	if ((command = (char *)malloc(datalen)) == NULL) {
		fprintf(stderr, "Error allocating memory\n");
		exit(-1);
	}

	memset(command, 0, datalen);

	if (fread(command, 1, datalen, stdin) != datalen) {
		fprintf(stderr, "Error reading set command from stdin\nexiting\n");
		free(command);
		exit(-1);
	}

	tmp = setcmd;
	setcmd = (char *)malloc(strlen(setcmd) + datalen);
	memset(setcmd, 0, strlen(setcmd) + datalen);
	strncat(setcmd, tmp, strlen(tmp));
	strncat(setcmd, command, datalen);
	free(tmp);
	free(command);
	return setcmd;
}

char *check_pipe(char *command)
{
	char *pipe;

	if ((pipe = strchr(command, '|')) == NULL)
		return command;

	*pipe = 0; // separate out command

	set_pipe_command(pipe+1);

	return trim(command);
}

void set_pipe_command(char *command)
{
	while (isspace(command[0]))
		command++;

	if ((strstr(command, "grep")) == command) {
		pipe_command.type = GREP;
		pipe_command.args = trim(command+5);
	} else if ((strstr(command, "sort")) == command) {
		pipe_command.type = SORT;
		pipe_command.args = command+5;
	} else {
		pipe_command.type = NONE;
		pipe_command.args = NULL;
	}
}

int communicate(char *msg)
{
	int i, numbytes, pos, used, len;
	char *buffer,tmp[BUFSIZ];

	i = numbytes = pos = used = 0;
	buffer = (char *)malloc(BUFSIZ);
	len = BUFSIZ;

	if (with_server == -1 || (strncmp(msg, "get", 3) == 0)) {
		for (i = 0; i < num_connections; i++) {
			if (send(socks[i], msg, strlen(msg), 0) == -1) {
				fprintf(stderr, "Error sending data to server\n");
				return -1;
			}

			if (i_verbose) printf("Response from %d: %s\n", i, get_servername(i));

			do {
				memset(&tmp, 0, BUFSIZ);
				if ((numbytes = recv(socks[i], &tmp, BUFSIZ, 0)) == -1) {
					fprintf(stderr, "Error reading from server\n");
					return -1;
				}
				if(enbuffer(&buffer, &used, &len, tmp, numbytes) == -ENOMEM) return -ENOMEM;
			} while (check_end_mc_response(tmp) == 0);
		}
	} else {
		if (send(socks[with_server], msg, strlen(msg), 0) == -1) {
			fprintf(stderr, "Error sending data to server\n");
			return -1;
		}

		do {
			memset(&tmp, 0, BUFSIZ);
			if ((numbytes = recv(socks[with_server], &tmp, BUFSIZ, 0)) == -1) {
				fprintf(stderr, "Error reading from server\n");
				return -1;
			}
			if(enbuffer(&buffer, &used, &len, tmp, numbytes) == -ENOMEM) return -ENOMEM;
		} while (check_end_mc_response(tmp) == 0);
	}

	buffer = do_pipe_cmd(buffer, &used);

	write(fileno(stdout), buffer, used);

	free(buffer);

	return 0;
}

char *do_pipe_cmd(char *data, int *len)
{
	char *out;

	if (pipe_command.type == GREP) {
		out = pgrep(pipe_command.args, data, len);
	} /*else if (pipe_command.type == SORT) {

	}*/ else {
		out = data;
	}

	pipe_command.type = NONE;
	pipe_command.args = NULL;

	return out;
}

int enbuffer(char **buffer, int *used, int *len, char *data, int data_len)
{
	char *tmp;


	if ((*len - *used) < (BUFSIZ >> 1)) { /* if less than 1/2 of BUFSIZ remains, reallocate */
#ifdef DEBUG
		fprintf(stderr, "buffer too small, calling realloc() to double size\n");
#endif
		tmp = (char *)realloc(*buffer, (*len * 2));
		*len *= 2;
		if (tmp == NULL) {
			/* mem allocation error */
			/* Should probably kill this thead or something like that */
			fprintf(stderr, "Error allocating memory, could not realloc()\n");
			return -ENOMEM;
		}
		*buffer = tmp;
	}

	/* copy data to the end of the existing data */
	memmove((*buffer + *used), data, data_len);
	*used += data_len;

	return 0;
}

int internal_command(char *s)
{
	char *t;
	int i;

	if (strncmp(s, "quit", 4) == 0) {
		exit(0);
	} else if (strncmp(s, "help", 4) == 0) {
		printf("list:          list connected servers\n");
		printf("help:          this help\n");
		printf("with <number>: bind commands to single server <number>\n");
		printf(" note that with will never bind 'get' or 'gets' calls\n");
		return 1;
	} else if (strncmp(s, "with", 4) == 0) {
		sscanf(s, "with %d", &with_server);
		t = const_cast<char *>(get_active_servername());
		printf("active server is now %d: %s\n", with_server, t);
		free(t);
		return 1;
	} else if (strncmp(s, "list", 4) == 0) {
		for (i = 0; i < num_connections; i++) {
			t = const_cast<char *>(get_servername(i));
			printf(" %d: %s\n", i, t);
			free(t);
		}
		return 1;
	} else if (strncmp(s, "verbose", 7) == 0) {
		if (i_verbose == 0) i_verbose = 1;
		else i_verbose = 0;
		printf("verbose: %s\n", ((i_verbose) ? "on" : "off"));
		return 1;
	}
	return 0;
}

int check_end_mc_response(char *buf)
{
	int i = 0;
	char *response;

	while ((response = responses[i++]) != NULL) {
		if (strstr(buf, response) != NULL) return 1;
	}

	return 0;
}

int parseopts(int argc, char **argv)
{
	Configuration *config = Configuration::get_instance();
	int o;

	while ((o = getopt(argc, argv, "vhc:s:")) != -1) {
		switch(o) {
			case 'v':
				verbose = 1;
				break;
			case 'c':
				conf_file = optarg;
				break;
			case 's':
				try {
					config->add_host(optarg);
				} catch (...) {
					usage(argv[0]);
				}
				break;
			case 'h':
			case '?':
			default:
				usage(argv[0]);
				exit(-1);
		}
	}
	return 0;
}

void usage(char *name)
{
	printf ("usage: %s [-v] [-c /path/to/config/file] [[-s server1:port] [-s srver2:port] [...]] [-h]\n", name);
}

int initialize(void)
{
	if (configure() != 0) return -1;
	if (connect_serverlist() != 0) return -1;
	return 0;
}

int configure(void)
{
	Configuration *mciconfig = Configuration::get_instance();
#ifdef DEBUG
	yydebug = 1;
#endif

	if (conf_file != NULL) yyin = fopen(conf_file, "r");
	else yyin = fopen("./mci.conf", "r");

	if (yyin != NULL)
		yyparse();

	if ((socks = (int *)malloc(sizeof(int) * mciconfig->get_as_int("max_connections"))) == NULL) return -ENOMEM;
	return 0;
}

int connect_serverlist(void)
{
	int sockfd;
	Configuration *config = Configuration::get_instance();

	if (config->num_hosts() < 1) return -1;

	for (std::vector<HostEnt>::const_iterator i = config->hosts_begin(); i != config->hosts_end(); i++) {
		HostEnt e = *i;
		if ((sockfd = socksify(e.host.c_str(), e.port)) == -1) {
			fprintf(stderr, "Error with socks connection\n");
			return -1;
		} else if (sockfd == 0) {
			// don't use socks
			if ((socks[num_connections++] = open_connection(e.host.c_str(), e.port)) == -1) {
				fprintf(stderr, "Error connecting to socket\n");
				return -1;
			}
		} else {
			// use socks
			socks[num_connections++] = sockfd;
		}
	}

	return 0;
}

int cleanup(void)
{
	int i;

	for (i = 0; i < num_connections; i++) {
		close(socks[i]);
	}

	return 0;
}

/* returns pointer to newly allocated string -- caller must free */
const char *get_active_servername(void)
{
	return get_servername(with_server);
}

/* returns pointer to newly allocated string -- caller must free */
const char *get_servername(int snum)
{
	std::stringstream server;
	Configuration *config = Configuration::get_instance();

	if (snum < 0) {
		server << "All";
	} else {
		try {
			HostEnt h = config->host_at(snum);
			server << h.host << ":" << h.port;
		} catch (std::out_of_range &e) {
			std::cerr << "Host index " << snum << " is out of range, max index is " << config->num_hosts()-1 << std::endl;
			server << "";
		}
	}

	return server.str().c_str();
}
