from PyQt4 import QtCore as core

class HostEntry:
    def __init__(self):
        self.port_regex = core.QRegExp("[1-9][0-9]{0,4}")
        self.host = None
        self.port = None

    def __del__(self):
        del host
        del port
