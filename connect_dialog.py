from PyQt4 import QtGui as qt, QtCore as core
from ui_connect_dialog import Ui_ConnectDialog
from hostentry import HostEntry

class ConnectDialog(qt.QDialog):
    def __init__(self, parent):
        qt.QDialog.__init__(self, parent)
        self.servers = []
        self.ui = Ui_ConnectDialog()
        self.ui.setupUi(self)

        self.ui.addserver_button.clicked.connect(self.addServerClicked)
        self.ui.buttonBox.button(qt.QDialogButtonBox.Ok).clicked.connect(parent.memcache_connect)
        self.ui.scrolling_contents.setLayout(qt.QVBoxLayout())
        self.ui.scrolling_contents.layout().setContentsMargins(0, 0, 0, 0)

    def addServerClicked(self):
        self.addHostEntry()

    def retab(self):
        last = qt.QWidget()
        qt.QWidget.setTabOrder(self.ui.buttonBox, self.ui.addserver_button)
        last = self.ui.addserver_button

        for i in self.servers:
            qt.QWidget.setTabOrder(last, i.host)
            qt.QWidget.setTabOrder(i.host, i.port)
            last = i.port
        qt.QWidget.setTabOrder(last, self.ui.buttonBox)

    def addHostEntry(self):
        host = HostEntry()
        host_widget = qt.QWidget()

        self.ui.scrolling_contents.layout().addWidget(host_widget)
        host = HostEntry()
        server_info_horiz_layout = qt.QHBoxLayout(host_widget)
        server_info_horiz_layout.setObjectName("server_info_horiz_layout %d" % (len(self.servers)))
        server_info_horiz_layout.setContentsMargins(0, 0, 0, 0)
        host_label = qt.QLabel(host_widget)
        host_label.setObjectName("host_label %d" % (len(self.servers)))
        host_label.setText("Host %d: " % (len(self.servers)+1))

        server_info_horiz_layout.addWidget(host_label)

        host.host = qt.QLineEdit(host_widget)
        host.host.setObjectName("host_edit %d" % (len(self.servers)))

        server_info_horiz_layout.addWidget(host.host)

        host_port_spacer = qt.QSpacerItem(28, 20, qt.QSizePolicy.Minimum, qt.QSizePolicy.Minimum)

        server_info_horiz_layout.addItem(host_port_spacer)

        port_label = qt.QLabel(host_widget)
        port_label.setObjectName("port_label %d" % (len(self.servers)))
        port_label.setText("Port %d: " % (len(self.servers)+1))

        server_info_horiz_layout.addWidget(port_label)

        host.port = qt.QLineEdit(host_widget)
        host.port.setObjectName("port_edit %d" % (len(self.servers)))
        host.port.setEnabled(True)
        sizePolicy1 = qt.QSizePolicy(qt.QSizePolicy.Fixed, qt.QSizePolicy.Fixed)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(host.port.sizePolicy().hasHeightForWidth())
        host.port.setSizePolicy(sizePolicy1)
        host.port.setMaximumSize(core.QSize(70, 16777215))
        host.port.setMaxLength(5)

        host.port.setValidator(qt.QRegExpValidator(host.port_regex, self))

        server_info_horiz_layout.addWidget(host.port)
        host.host.setFocus()
        self.servers.append(host)

        self.retab()

    def __del__(self):
        for h in self.servers:
            del h
        del ui

    def changeEvent(self, e):
        if e.type() == core.QEvent.LanguageChange:
            self.ui.retranslateUi(self)
