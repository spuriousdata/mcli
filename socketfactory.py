import socket
import socks
from main import config

class socketfactory:
    @classmethod
    def get_socket(family=2, type=1, proto=0, _sock=None):
        if config.has_section('socks'):
            return socks.socks(family, type, proto, _sock)
        else: return socket.socket(family, type, proto, _sock)

