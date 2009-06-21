from PyQt4 import QtGui as qt, QtCore as core
from ui_mci import Ui_McIClass
from connect_dialog import ConnectDialog

class Mci(qt.QMainWindow):
    def __init__(self, parent):
        qt.QMainWindow.__init__(self)
        self.ui = Ui_McIClass()
        self.ui.setupUi(self)

        self.connect(self.ui.action_quick_connect, core.SIGNAL("triggered()"), self, core.SLOT("openConnectDialog()"))
        self.connect(self.ui.add_button, core.SIGNAL("clicked()"), self, core.SLOT("addClicked()"))
        self.connect(self.ui.delete_button, core.SIGNAL("clicked()"), self, core.SLOT("deleteClicked()"))
        self.connect(self.ui.get_button, core.SIGNAL("clicked()"), self, core.SLOT("getClicked()"))
        self.connect(self.ui.flushall_button, core.SIGNAL("clicked()"), self, core.SLOT("flushallClicked()"))

    def openConnectDialog(self):
        if self.connect_dialog == None:
            self.connect_dialog = ConnectDialog(self)

        self.connect_dialog.show()
        self.connect_dialog.raise_()
        self.connect_dialog.activateWindow()

        self.connect(self.connect_dialog.mc, core.SIGNAL("hasNewStats()"), self, core.SLOT("displayStats()"))

    def displayStats(self):
        pass

    def addClicked(self):
        print "addClicked() called"

    def getClicked(self):
        print "getClicked() called"

    def deleteClicked(self):
        print "deleteClicked() called"

    def flushallClicked(self):
        print "flushallClicked() called"

    def __del__(self):
        del self.ui
        del self.connect_dialog
