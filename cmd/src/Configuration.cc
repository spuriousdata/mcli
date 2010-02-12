#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <stdexcept>

#include "HostEnt.h"
#include "Configuration.h"

Configuration *Configuration::instance = NULL;

Configuration::Configuration()
{
	set_value("max_connections", NUMRESP);
	set_value("memcache_protocol", MC_PROTO_TEXT);
	set_value("use_socks", false);
	set_value("socks_proto", SOCKS_DEFAULT_PROTO);
	set_value("socks_port", SOCKS_DEFAULT_PORT);
	set_value("socks_dns", false);
	set_value("socks_host", NULL);
	set_value("socks_username", NULL);
	set_value("socks_password", NULL);
	set_value("verbose", false);
}

Configuration *Configuration::get_instance()
{
	if (instance == NULL)
		instance = new Configuration();
	return instance;
}

void Configuration::set_value(const std::string key, unsigned int value)
{
	std::stringstream s;
	s << value;
#ifdef DEBUG
	std::cerr << "Setting configuration value " << key << " to " << s.str() << std::endl;
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
	std::cerr << "Setting configuration value " << key << " to " << value << std::endl;
#endif
	data[key] = value;
}

void Configuration::set_value(const std::string key, bool value)
{
	std::stringstream s;
	s << std::boolalpha << value;
#ifdef DEBUG
	std::cerr << "Setting configuration value " << key << " to " << std::boolalpha << s.str() << std::endl;
#endif
	set_value(key, s.str());
}

void Configuration::add_host(std::string host, unsigned short port)
{
	hosts.push_back(HostEnt(host,port));
}

void Configuration::add_host(std::string hoststr)
{
	size_t colon;
	std::stringstream portstr;
	std::string host;
	
	unsigned short port;

	if ((colon = hoststr.find(':')) == std::string::npos)
		throw std::invalid_argument(std::string(hoststr).append(" is not a valid server"));

	host = hoststr.substr(0, colon);
	portstr << hoststr.substr(colon+1);
	portstr >> port;

	add_host(host,port);
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

const bool Configuration::toggle_bool(const std::string key)
{
	if (get_as_bool(key) == true) set_value(key, false);
	set_value(key, true);

	return get_as_bool(key);
}

