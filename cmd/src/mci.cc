#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cctype>
#include <readline/readline.h>
#include <readline/history.h>
#include "mci.h"
#include "util.h"
#include "rl_complete.h"
#include "Configuration.h"
#include "HostEnt.h"
#include "MemcacheClient.h"
#include "InputInterpreter.h"
#ifdef HAVE_LIBPCRE
#include "pgrep.h"
#endif

int *socks, num_connections = 0;
int with_server = -1;
char *conf_file = NULL;
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

MemcacheClient *mc;
InputInterpreter *ii;

int main(int argc, char **argv)
{
	int port;
	char *msg, *command = (char *)NULL;
	Configuration *config;

	if (parseopts(argc, argv) != 0) return -1;
	if (initialize() == -1) return -1;
	initialize_readline();

	config = Configuration::get_instance();

	while (1) {

		if (command) {
			free(command);
			command = (char *)NULL;
		}

		if (mc->data_exists()) {
			for (int i = 0; i < config->num_hosts(); i++ ) {
				if (mc->get_data(i).length() > 0) {
					std::cout << "From " << config->host_at(i).host << ":" << config->host_at(i).port << std::endl;
					std::cout << mc->get_data(i) << std::endl;
				}
			}
		}

		command = readline("memcache> ");

		if (command && *command) {
			add_history(command);

			if (internal_command(command)) continue;
			//command = check_pipe(command);
			
			ii->interpret(command);
		}
	}

	cleanup();
	return 0;
}

void set_pipe_command(char *command)
{
	return;
	/*
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
	*/
}

char *do_pipe_cmd(char *data, int *len)
{
	char *out;
#ifndef HAVE_LIBPCRE
	return data;
#else
	if (pipe_command.type == GREP) {
		out = pgrep(pipe_command.args, data, len);
	} /*else if (pipe_command.type == SORT) {

	}*/ else {
		out = data;
	}

	pipe_command.type = NONE;
	pipe_command.args = NULL;

	return out;
#endif
}

int internal_command(char *s)
{
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
		printf("active server is now %d: %s\n", with_server, get_active_servername());
		return 1;
	} else if (strncmp(s, "list", 4) == 0) {
		for (i = 0; i < num_connections; i++) {
			printf(" %d: %s\n", i, get_servername(i));
		}
		return 1;
	} else if (strncmp(s, "verbose", 7) == 0) {
		Configuration::get_instance()->toggle_bool("verbose");
		printf("verbose: %s\n", Configuration::get_instance()->get_value("verbose").c_str());
		return 1;
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
				config->set_value("verbose", true);
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
	mc = new MemcacheClient(Configuration::get_instance()->get_hosts());
	ii = new InputInterpreter(mc);

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

	return 0;
}

void cleanup(void)
{
	delete mc;
	delete ii;
}

const char *get_active_servername(void)
{
	return get_servername(with_server);
}

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
