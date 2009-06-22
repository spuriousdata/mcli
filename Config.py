from ConfigParser import SafeConfigParser, NoOptionError, NoSectionError
from getopt import getopt, GetoptError
import sys


class Configuration(SafeConfigParser):
    def __init__(self, defaults = {}):
        SafeConfigParser.__init__(self, defaults)
        self.verbose = False
        self.cfile = './mci.ini'
        try:
            opts, args = getopt(sys.argv[1:], "vhc:s:", ["help"])
        except GetoptError:
            self.usage()
            sys.exit(-1)

        for o,a in opts:
            if o == '-v':
                self.verbose = True
                self.set('general', 'verbose', 'True')
            if o == '-c': self.cfile = a
            if o == '-s': self.addserver(a)
            for o in ('-h', '--help'):
                self.usage()
                sys.exit(-1)

        self.configure(self.cfile)

    def addserver(self, server):
        s = self.get('memcache', 'servers')
        if s is not None:
            s += "," + server
        else:
            s = server
        self.set('memcache', 'servers', s)

    def configure(self, _file):
        self.configfile = _file
        try:
            self.readfp(open(self.configfile))
        except IOError, e:
            print "Error opening file %s, %s" % (self.cfile, e)
            sys.exit(-1)

    def get(self, section, name, raw=False, vars=None):
        try:
            val = SafeConfigParser.get(self, section, name, raw, vars)
            if val.lower() == 'true': val = True
            elif val.lower() == 'false': val = False
            return val
        except NoSectionError:
            return None
        except NoOptionError:
            return None

    def usage(self):
        print """usage: %s [-v] [-c /path/to/config/file] \
            [[-s server1:port] [-s server2:port] [...]] [-h|--help]\n""" % (sys.argv[0])

config = Configuration()


