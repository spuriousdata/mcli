#ifndef __MEMCACHECLIENT_H__
#define __MEMCACHECLIENT_H__

#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include "HostEnt.h"

class Connection;
class ConnectionReader;

class MemcacheClient
{
	public:
	   	MemcacheClient(const std::vector<HostEnt>& hosts);
		void set(std::string key, std::string flags, std::string exptime, std::string length, std::string nr = "");
		void add(std::string key, std::string flags, std::string exptime, std::string length, std::string nr = "");
		void replace(std::string key, std::string flags, std::string exptime, std::string length, std::string nr = "");
		void append(std::string key, std::string flags, std::string exptime, std::string length, std::string nr = "");
		void prepend(std::string key, std::string flags, std::string exptime, std::string length, std::string nr = "");
		void cas(std::string key, std::string flags, std::string exptime, std::string length, std::string nr = "");
		void get(std::string key);
		void get(std::vector<std::string>& keys);
		void gets(std::string key);
		void gets(std::vector<std::string>& keys);
		void _delete(std::string key, std::string when, std::string nr = "");
		void incr(std::string key);
		void decr(std::string key);
		void stats();
		void flush_all();
		void version();
		void quit();
		const bool data_exists() const;
		const std::string get_data(int host_index) const;
		

	private:
		int which_server;
		std::vector<Connection*> connections;
		ConnectionReader *reader;
		void store(std::string cmd, std::string key, std::string flags, std::string exptime, std::string length, std::string nr);
		void retrieve(std::string cmd, std::vector<std::string>& keys);
		void send(std::string& cmd);
		void send(const std::string& cmd);
};

class MemcacheException : public std::exception
{
	public:
		MemcacheException(int code, std::string s) throw() : std::exception(), msg(s), code(code) {}
		virtual const char *what() const throw()
		{
			std::stringstream s;
			s << "Error(" << code << ") " << msg;
			return s.str().c_str();
		}
		~MemcacheException() throw() {}

	private:
		std::string msg;
		int code;
};

#endif
