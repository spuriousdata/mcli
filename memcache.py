import socketfactory
import socket

class Memcache(object):
    def __init__(self, servers = []):
        self.sockets = []
        self.servers = servers

    def connect(self):
        for server in self.servers:
            sock = socketfactory.get_socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((server.host.text(), int(server.port.text())))
            self.sockets.append(sock)

    def disconnect(self):
        for socket in self.sockets:
            socket.close()
            del socket
        
    def send(self, data):
        for sock in self.sockets:
            self.safe_sendall(sock, data)
            
    def safe_sendall(self, sock, data):
        bytes = len(data)
        while True:
            sent = sock.send(data)
            if sent == 0:
                raise IOError("Trouble sending %d bytes to socks server" % (bytes))
            bytes -= sent
            if bytes == 0: break
    
    def recv(self):
        BUFSIZ = 4096
        resp = {}
        for i in range(0,len(self.sockets)):
            data = str()
            while True:
                tmp = self.sockets[i].recv(BUFSIZ)
                data += tmp
                if len(tmp) < BUFSIZ:
                    break
            resp[self.servers[i].host.text() + ":" + self.servers[i].port.text()] = data
        return resp

    def get_stats(self):
        self.send("STATS\r\n")
        return self.recv()

    def __del__(self):
        for s in self.sockets:
            del s
        for s in self.servers:
            del s
