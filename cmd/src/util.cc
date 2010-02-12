#include <string>
#include "util.h"

std::string rtrim(std::string s)
{
	size_t e;

	e = s.find_last_not_of(" \t\r\n");
	return s.substr(0, e+1);
}

std::string ltrim(std::string s)
{
	size_t e;

	e = s.find_first_not_of(" \t\r\n");
	return s.substr(e);
}

std::string trim(std::string s)
{
	return ltrim(rtrim(s));
}
