#ifndef PARSEOPTS_H
#define PARSEOPTS_H

#define MCI_DEFAULT_CONFIG_FILE "./mci.conf"

class po::variables_map;
po::variables_map parseopts(int argc, char **argv);

#endif
