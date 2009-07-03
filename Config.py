from ConfigParser import SafeConfigParser, NoOptionError, NoSectionError
from getopt import getopt, GetoptError
import sys


class Configuration(SafeConfigParser):
    def __init__(self, defaults={}):
        SafeConfigParser.__init__(self, defaults)
        self.verbose = False
        self.cfile = '/Users/momalley/Dev/mci/branches/pymci/mci.ini'
        try:
            opts, args = getopt(sys.argv[1:], "vhc:s:", ["help"])
        except GetoptError:
            self.usage()
            sys.exit(-1)

        for o, a in opts:
            if o == '-v':
                self.verbose = True
                self.set('general', 'verbose', 'True')
            if o == '-c': self.cfile = a
            if o == '-s': self.addserver(a)
            for o in ('-h', '--help'):
                self.usage()
                sys.exit(-1)

        self.configure(self.cfile)

    def configure_proxy_settings(self, properties_dialog):
        self.set('socks', 'protocol', str(properties_dialog.ui.socks_proto.value()))
        self.set('socks', 'hostname', str(properties_dialog.ui.socks_hostname.text()))
        self.set('socks', 'port', str(properties_dialog.ui.socks_port.text()))
        self.set('socks', 'dns', str(properties_dialog.ui.socks_dns.isChecked()))
        if properties_dialog.ui.socks_proto.value() == 5 and str(properties_dialog.ui.socks_username.text()) is not "":
            self.set('socks', 'username', str(properties_dialog.ui.socks_username.text()))
            self.set('socks', 'password', str(properties_dialog.ui.socks_password.text()))
        self.write(sys.stdout)

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


