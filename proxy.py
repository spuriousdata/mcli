import socket

class Proxy(socket.socket):
    def __init__(self, family=2, type=1, proto=0, _sock=None):
        super().__init__(family, type, proto, _sock):
