import socket
from Config import config
from struct import Struct

class Socks(socket.socket):
    BUFSIZ = 4096
    init_auth_pkt = Struct("BBBB")
    init_noauth_pkt = Struct("BBB")
    resp_pkt = Struct("BB")
    unpw_auth_pkt = Struct("BBsBs")
    connect_dns_pkt = Struct("BBBBBsh")
    connect_ip4_pkt = Struct("BBBBIh")

    def __init__(self, family=2, type=1, proto=0):
        socket.socket.__init__(self, int(family), type, proto)

    def connect(self, *args):
        host,port = args

        socks_host = config.get('socks', 'hostname')
        socks_port = config.get('socks', 'port')
        socks_use_dns = config.get('socks', 'dns')
        socks_proto = int(config.get('socks', 'protocol'))
        if socks_proto == None: socks_proto = 5

        if socks_proto == 5:
            socks_user = config.get('socks', 'userame')
            socks_pass = config.get('socks', 'password')

        if config.get('general', 'verbose') == True:
            print "Connecting to SOCKS%d proxy" % socks_proto
        super(Socks,self).connect((socks_host, int(socks_port)))

        data = None
        if socks_user is not None:
            # proto = 5, number_of_methods, method 0 -- no_auth, method 2 -- un/pw auth
            data = Socks.init_auth_pkt.pack(5, 2 , 0, 2)
        else:
            # proto = 5, number_of_methods, method 0 -- no_auth
            data = Socks.init_noauth_pkg.pack(5, 1, 0)

        self.safe_sendall(data)

        resp = Socks.resp_pkt.unpack(self.recv(Socks.BUFSIZ))

        if resp[0] != 5:
            self.close()
            raise IOError("Communication error")

        if resp[1] == 0xFF:
            self.close()
            raise IOError("No acceptable SOCKS authentication methods")

        if resp[1] == 2:
            # un/pw auth
            data = Socks.unpw_auth_pkt.pack (
                    1, len(socks_user), socks_user, 
                    len(socks_pass), socks_pass
            )

            self.safe_sendall(data)

            resp = Socks.resp_pkt.unpack(self.recv(Socks.BUFSIZ))

            if resp[0] != 1:
                self.close()
                raise IOError("Protcol error. Un/Pw negotiation failed")

            if resp[1] != 0:
                self.close()
                raise IOError("Error connecting to socks proxy, bad username or password")

            # We've made it, we're good. We can now connect to the remote server
            if socks_use_dns:
                data = Socks.connect_pkt.pack(5, 1, 0, 3, len(host), host, socket.htons(port))
            else:
                packed_ip = socket.inet_aton(socket.gethostbyname(host))
                data = Socks.connect_pkt.pack(5, 1, 0, 1, packed_ip, socket.htons(port))

            self.safe_sendall(data)
            resp = Socks.resp_pkt.unpack(self.recv(Socks.BUFSIZ))

            if resp[0] != 5:
                raise IOError("Protocol error")
            if resp[1] != 0:
                raise IOError("socks connect() call failed")

    def safe_sendall(self, data):
        bytes = len(data)
        while True:
            sent = self.send(data)
            if sent == 0:
                raise IOError("Trouble sending %d bytes to socks server" % (bytes))
            bytes -= sent
            if bytes == 0: break