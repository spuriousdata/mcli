#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "parseopts.hpp"

#include <iostream>

int main(int argc, char **argv)
{
	try {
		po::variables_map opts = parseopts(argc, argv);
	} catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    } catch(...) {
        std::cerr << "Exception of unknown type!\n";
        return 1;
    }
	
	return 0;
}
