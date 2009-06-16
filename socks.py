import socket
from main import config
from struct import Struct

class socks(socket.socket):
    BUFSIZ = 4096
    init_auth_pkt = Struct("BBBB")
    init_noauth_pkt = Struct("BBB")
    init_resp_pkt = Struct("BB")

    def __init__(self, family=2, type=1, proto=0):
        super().__init__(self, family, type, proto)

    def connect(self, *args):
        host,port = args[0]

        socks_host = config.get('socks', 'hostname')
        socks_port = config.get('socks', 'port')
        socks_proto = config.get('socks', 'protocol')
        if socks_proto == None: socks_proto = 5
        
        if socks_proto == 5:
            socks_user = config.get('socks', 'userame')
            socks_pass = config.get('socks', 'password')

        if config.get('general', 'verbose') == True:
            print "Connecting to SOCKS%d proxy" % socks_proto
        super().connect((socks_host, socks_port))

        size = 0
        data = None
        if socks_user is not None:
            # proto = 5, number_of_methods, method 0 -- no_auth, method 2 -- un/pw auth
            size = 4
            data = socks.init_auth_pkt.pack(5, 2 , 0, 2)
        else:
            # proto = 5, number_of_methods, method 0 -- no_auth
            size = 3
            data = socks.init_noauth_pkg.pack(5, 1, 0)

        if self.send(data) != size:
            raise IOError("problem sending %d bytes" % size)

        resp = socks.init_resp_pkt.unpack(self.recv(socks.BUFSIZ))

        if resp[0] != 5:
            raise IOError("Communication error")

        if resp[1] == 0xFF:
            raise IOError("No acceptable SOCKS authentication methods")

        :w





        





