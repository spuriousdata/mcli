import socket
import socks
from Config import config

def get_socket(family=2, type=1, proto=0):
    if config.has_section('socks'):
        return socks.socks(family, type, proto)
    else: return socket.socket(family, type, proto)

