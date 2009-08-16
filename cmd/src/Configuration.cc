#include <string>
#include <sstream>
#include <iostream>
#include <map>

#include "hostent.h"
#include "Configuration.h"

Configuration::Configuration()
{
	set_value("max_connections", Configuration::NUMRESP);
	set_value("use_socks", false);
	set_value("socks_proto", Configuration::SOCKS_DEFAULT_PROTO);
	set_value("socks_port", Configuration::SOCKS_DEFAULT_PORT);
	set_value("socks_dns", false);
	set_value("socks_host", NULL);
	set_value("socks_username", NULL);
	set_value("socks_password", NULL);
}

void Configuration::set_value(const std::string key, unsigned int value)
{
	std::stringstream s;
	s << value;
#ifdef DEBUG
	std::cerr << "Setting configuration value " << key << " to " << s.str();
#endif
	set_value(key, s.str());
}

void Configuration::set_value(const std::string key, int value)
{
	set_value(key, (unsigned int)value);
}

void Configuration::set_value(const std::string key, const std::string value)
{
#ifdef DEBUG
	std::cerr << "Setting configuration value " << key << " to " << value;
#endif
	data[key] = value;
}

void Configuration::set_value(const std::string key, bool value)
{
	std::stringstream s;
	s << std::boolalpha << value;
	set_value(key, s.str());
}

const std::string Configuration::get_value(const std::string key) const
{
	std::map<const std::string, std::string>::const_iterator i = data.find(key);

	if (i != this->data.end())
#ifdef DEBUG
	{
		std::cerr << "Getting configuration value for " << key << " as " << i->second << std::endl;
		return i->second;
	} else {
		std::cerr << "Value for key " << key << " doesn't exist, returning NULL\n";
		return NULL;
	}
#else
		return i->second;
	else return NULL;
#endif
}

const int Configuration::get_as_int(const std::string key) const
{
	int i;
	std::stringstream s(get_value(key));
	
	s >> i;
#ifdef DEBUG
	std::cerr << "Getting confirguration value for " << key << " as integer " << i << std::endl;
#endif

	return i;
}

const bool Configuration::get_as_bool(const std::string key) const
{
	bool b;
	std::stringstream s(get_value(key));

	s >> std::boolalpha >> b;
#ifdef DEBUG
	std::cerr << "Getting confirguration value for " << key << " as bool " << b << std::endl;
#endif

	return b;
}
