#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <string>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	try {
		po::options_description desc("usage");
		desc.add_options()
			("help", "produce this help message")
			("server", po::value<string>(), "memcache server to connect to")
			("verbose", "turn on verbose output")
			("config", po::value<string>(), "/path/to/mci.conf")
		;
		
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
		
		if (vm.count("help")) {
			cout << desc << "\n";
			return -1;
		}
		
		if (vm.count("server") > 0) {
			cout << "Connecting to " << vm["server"].as<string>() << "\n";
		} else {
			cout << "No servers to connect to\n";
		}
		
	} catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    } catch(...) {
        cerr << "Exception of unknown type!\n";
    }
	
	return 0;
}
