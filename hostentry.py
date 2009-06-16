class HostEntry:
    def __init__(self):
        self.port_regex = QRegExp("[1-9][0-9]{0,4}")

    def __del(self):
        del host
        del port
