from ui_proxy_config import Ui_ProxyConfig
from PyQt4 import QtGui as qt, QtCore as core
from Config import config

class ProxyConfig(qt.QDialog):
    def __init__(self, parent):
        qt.QDialog.__init__(self, parent)
        self.ui = Ui_ProxyConfig()
        self.ui.setupUi(self)

        self.ui.socks_proto.valueChanged[int].connect(self.toggle_unpw)
        self.ui.buttonBox.button(qt.QDialogButtonBox.Ok).clicked.connect(
            parent.configure_proxy_settings
        )
        self.ui.socks_port.setValidator(
            qt.QRegExpValidator(core.QRegExp("[1-9][0-9]{0,4}"), self)
        )

    def toggle_unpw(self, val):
        if config.get('general', 'verbose'):
            print "socks protocol version is %d" % val
        enable = False
        if val == 5:
            enable = True
        self.ui.socks_username_label.setEnabled(enable)
        self.ui.socks_username.setEnabled(enable)
        self.ui.socks_password_label.setEnabled(enable)
        self.ui.socks_password.setEnabled(enable)

