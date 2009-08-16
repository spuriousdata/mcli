#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <map>
#include <string>

class Configuration
{
	public:
		static const int NUMRESP = 10;
		static const int MC_DEFAULT_PORT = 11227;
		static const int SOCKS_DEFAULT_PORT = 1080;
		static const int SOCKS_DEFAULT_PROTO = 5;

		Configuration();
		void set_value(const std::string key, const std::string v);
		void set_value(const std::string key, unsigned int value);
		void Configuration::set_value(const std::string key, int value);
		void set_value(const std::string key, bool value);
		const std::string get_value(const std::string key) const;
		const int get_as_int(const std::string key) const;
		const bool get_as_bool(const std::string key) const;

	private:
		std::map<const std::string, std::string> data;

};

#endif
