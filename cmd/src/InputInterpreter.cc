#include <string>
#include "InputInterpreter.h"
#include "MemcacheClient.h"
#include "util.h"

void InputInterpreter::interpret(const char *input)
{
	std::string inp = trim(input);
	std::string first;
	size_t space;

	if (inp.find('|') != std::string::npos)
		; // do pipe commeand

	if ((space = inp.find(' ')) == std::string::npos)
		first = inp;
	else {
		first = inp.substr(0, space);
	}

	if (first == "stats")
		mc->stats();
}

/*
	switch (cmd) {
		case "set":
		case "add":
		case "replace":
		case "append":
		case "prepend":
		case "cas":
		case "incr":
		case "decr":
		case "get":
		case "gets":
		case "delete":
			return true;
		default:
			return false;
	}
*/
