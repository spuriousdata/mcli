#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <map>
#include <vector>
#include <string>

#include "HostEnt.h"

class Configuration
{
	public:
		static const int NUMRESP = 10;
		static const int MC_DEFAULT_PORT = 11227;
		static const int MC_PROTO_TEXT = 1;
		static const int MC_PROTO_BIN = 2;
		static const int SOCKS_DEFAULT_PORT = 1080;
		static const int SOCKS_DEFAULT_PROTO = 5;

		Configuration();
		static Configuration *get_instance();

		void set_value(const std::string key, const std::string v);
		void set_value(const std::string key, unsigned int value);
		void set_value(const std::string key, int value);
		void set_value(const std::string key, bool value);
		void add_host(std::string host, unsigned short port);
		void add_host(std::string hoststr);
		const std::string get_value(const std::string key) const;
		const int get_as_int(const std::string key) const;
		const bool get_as_bool(const std::string key) const;
		const bool toggle_bool(const std::string key);
		const int num_hosts() const {return hosts.size();}
		std::vector<HostEnt>::const_iterator hosts_begin() const {return hosts.begin();}
		std::vector<HostEnt>::const_iterator hosts_end() const {return hosts.end();}
		const HostEnt& host_at(int i) const {return hosts.at(i);}
		const std::vector<HostEnt>& get_hosts() const { return hosts; }

	private:
		std::map<const std::string, std::string> data;
		std::vector<HostEnt> hosts;
		static Configuration *instance;

};

#endif
