import socketfactory
import socket

class Memcache(object):
    def __init__(self, servers = []):
        self.sockets = []
        self.servers = servers

    def connect(self):
        for server in self.servers:
            sock = socketfactory.get_socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect(server.host.text(), int(server.port.text()))
            self.sockets.append(sock)

    def disconnect(self):
        for socket in self.sockets:
            socket.close()
            del socket

    def __del__(self):
        for s in self.sockets:
            del s
        for s in self.servers:
            del s
