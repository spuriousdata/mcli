from socketfactory import socketfactory
from main import config

class Memcache:
    def __init__(self, servers = []):
        self.sockets = []
        self.servers = servers

    def connect(self):
        for server in self.servers:
            socket = socketfactory.get_socket(socket.AF_INET, socket.SOCK_STREAM)
            socket.connect(server.host, server.port)
            self.sockets.append(socket)

    def disconnect(self):
        for socket in self.sockets:
            socket.close()
            del socket

    def __del__(self):
        for s in self.sockets:
            del s
        for s in self.servers:
            del s
