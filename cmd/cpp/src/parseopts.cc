#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <string>
#include <fstream>
#include "parseopts.hpp"

po::variables_map parseopts(int argc, char **argv)
{
	po::options_description generic("Command Line Options");
	generic.add_options()
		("help,h", "produce this help message")
		("verbose,v", "turn on verbose output")
		("config-file,c", po::value<std::string>()->default_value(MCI_DEFAULT_CONFIG_FILE), "/path/to/mci.conf")
	;

	po::options_description config("Configuration");
	config.add_options()
		("server,s", po::value< std::vector<std::string> >()->composing(), "memcache server to connect to")
		("max_connections", po::value<int>()->default_value(10), "Maximum number of connnections")
		("use_socks", po::value<bool>()->default_value(false), "Use a SOCKS proxy")
		("socks_proto", po::value<int>()->default_value(5), "SOCKS version 4|5")
		("socks_host", po::value<std::string>()->default_value("localhost"), "Hostname of SOCKS proxy")
		("socks_port", po::value<int>()->default_value(1080), "Port of SOCKS proxy")
	;
	
	po::options_description cmd_opts;
	cmd_opts.add(generic).add(config);
	
	po::options_description config_opts;
	config_opts.add(config);
	
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, cmd_opts), vm);

	std::ifstream ifs(vm["config-file"].as<std::string>().c_str());
	po::store(po::parse_config_file(ifs, config_opts), vm);
	po::notify(vm);

	return vm;
	
}
