import sys
import socket

class Memcache:
    def __init__(self, servers = None):
        self.sockets = {}
        self.verbose = False
        if servers is None:
            servers = {}
        for server in servers:
            socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            socket.connect

