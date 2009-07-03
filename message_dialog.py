from PyQt4.QtGui import QMessageBox as mb

class Message(object):
    def __init__(self, message):
        self.m = mb()
        self.m.setText(message)
        self.m.buttonClicked.connect(self.destroy)
        self.m.exec_()
        
    def destroy(self):
        del self.m
