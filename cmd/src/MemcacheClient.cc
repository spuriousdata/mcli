#include <vector>
#include <sstream>
#include <string>
#include "MemcacheClient.h"
#include "Connection.h"
#include "ConnectionReader.h"
#include "HostEnt.h"

MemcacheClient::MemcacheClient(const std::vector<HostEnt>& hosts) :
	which_server(-1)
{
	for (std::vector<HostEnt>::const_iterator i = hosts.begin(); i != hosts.end(); i++) {
		connections.push_back(new Connection(i->host.c_str(), i->port));
	}

	reader = new ConnectionReader(connections, this);
}

void MemcacheClient::set(std::string key, std::string flags, std::string exptime, std::string length, std::string nr)
{
	store("set", key, flags, exptime, length, nr);
}

void MemcacheClient::add(std::string key, std::string flags, std::string exptime, std::string length, std::string nr)
{
	store("add", key, flags, exptime, length, nr);
}

void MemcacheClient::replace(std::string key, std::string flags, std::string exptime, std::string length, std::string nr)
{
	store("replace", key, flags, exptime, length, nr);
}

void MemcacheClient::append(std::string key, std::string flags, std::string exptime, std::string length, std::string nr)
{
	store("append", key, flags, exptime, length, nr);
}

void MemcacheClient::prepend(std::string key, std::string flags, std::string exptime, std::string length, std::string nr)
{
	store("prepend", key, flags, exptime, length, nr);
}

void MemcacheClient::cas(std::string key, std::string flags, std::string exptime, std::string length, std::string nr)
{
	store("cas", key, flags, exptime, length, nr);
}

void MemcacheClient::store(std::string cmd, std::string key, std::string flags, std::string exptime, std::string length, std::string nr)
{
	Connection *c;
	std::stringstream s;

	if (which_server == -1)
		throw MemcacheException(-1, "STORE commands must be bound to a single server, see 'help store'");

	s << cmd << " " << key << " " << flags << " " << exptime << " " << length << " " << nr << "\r\n";
	send(s.str());
}

void MemcacheClient::get(std::string key) {}
void MemcacheClient::get(std::vector<std::string>& keys) {}
void MemcacheClient::gets(std::string key){}
void MemcacheClient::gets(std::vector<std::string>& keys){}
void MemcacheClient::_delete(std::string key, std::string when, std::string nr) {}
void MemcacheClient::retrieve(std::string cmd, std::vector<std::string>& keys) {}
void incr(std::string key) {}
void decr(std::string key) {}

void MemcacheClient::stats()
{
	send("stats\r\n");
}

void MemcacheClient::send(std::string &cmd)
{
	send(cmd);
}

void MemcacheClient::send(const std::string& cmd)
{
	Connection *c;
	
	if (which_server == -1) {
		for(std::vector<Connection*>::const_iterator i = connections.begin(); i != connections.end(); i++) {
			c = *i;
			c->send(cmd);
		}
	} else {
		c = connections.at(which_server);
		c->send(cmd);
	}
}

void MemcacheClient::flush_all() {}
void MemcacheClient::version() {}
void MemcacheClient::quit() {}

const bool MemcacheClient::data_exists() const
{
	bool has_data;

	reader->acquire_data_lock();
	has_data = (reader->get_data().size() > 0) ? true : false;
	reader->release_data_lock();
	return has_data;
}

const std::string MemcacheClient::get_data(int host_index) const
{

	reader->acquire_data_lock();
	std::vector< std::vector<char> >& data = reader->get_data();

	std::string data_str(data.at(host_index)[0], data.at(host_index).size());
	
	data.at(host_index).clear();
	reader->release_data_lock();
	return data_str;
}
