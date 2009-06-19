from ConfigParser import ConfigParser
from getopt import getopt, GetoptError
import sys, os

class Configuration(ConfigParser):
    def __init__(self):
        super().__init__(self)
        self.verbose = False
        self.cfile = './mci.ini'
        try:
            opts, args = getopt(sys.argv[1:], "vhc:s:", ["help"])
        except GetoptError:
            self.usage()
            sys.exit(-1)

        for o,a in opts:
            if o == '-v': self.verbose = True
            if o == '-c': self.cfile = a
            if o == '-s': self.addserver(a)
            for o in ('-h', '--help'):
                self.usage()
                sys.exit(-1)

        self.configure(self.cfile)

    def configure(self, _file):
        self.configfile = _file
        try:
            self.readfp(open(self.configfile))
        except IOError, e:
            print "Error opening file %s, %s" % (self.cfile, e)
            sys.exit(-1)

    def usage()
        print "usage: %s [-v] [-c /path/to/config/file] ",
            "[[-s server1:port] [-s server2:port] [...]] [-h|--help]\n" % (sys.argv[0])


    
