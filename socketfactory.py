import socket
import socks
from main import config

class socketfactory:
    def __init__(self, family=2, type=1, proto=0, _sock=None):
        if config.get('socks', 'use_socks'

