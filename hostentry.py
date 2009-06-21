from PyQt4 import QtCore as core

class HostEntry:
    def __init__(self):
        self.port_regex = core.QRegExp("[1-9][0-9]{0,4}")

    def __del(self):
        del host
        del port
