from PyQt4 import QtGui as qt
from ui_mci import Ui_McIClass
from connect_dialog import ConnectDialog
from proxy_config import ProxyConfig
from memcache import Memcache
from Config import config

class Mci(qt.QMainWindow):
    def __init__(self, parent):
        qt.QMainWindow.__init__(self)
        self.connect_dialog = None
        self.properties_dialog = None
        self.ui = Ui_McIClass()
        self.ui.setupUi(self)

        self.ui.action_quick_connect.triggered.connect(self.openConnectDialog)
        self.ui.action_properties.triggered.connect(self.openPropertiesDialog)
        self.ui.add_button.clicked.connect(self.addClicked)
        self.ui.delete_button.clicked.connect(self.deleteClicked)
        self.ui.get_button.clicked.connect(self.getClicked)
        self.ui.flushall_button.clicked.connect(self.flushallClicked)

    def openConnectDialog(self):
        if self.connect_dialog == None:
            self.connect_dialog = ConnectDialog(self)

        self.connect_dialog.show()
        self.connect_dialog.raise_()
        self.connect_dialog.activateWindow()

    def openPropertiesDialog(self):
        if self.properties_dialog == None:
            self.properties_dialog = ProxyConfig(self)

        self.properties_dialog.show()
        self.properties_dialog.raise_()
        self.properties_dialog.activateWindow()

    def configure_proxy_settings(self):
        config.configure_proxy_settings(self.properties_dialog)

    def memcache_connect(self):
        self.mc = Memcache(self.connect_dialog.servers)
        self.mc.connect()

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
