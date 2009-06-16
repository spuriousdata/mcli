from PyQt4 import QtGui, QtCore as core
from ui_connect_dialog import Ui_ConnectDialog

def ConnectDialog(QtGui.QDialog):
    def __init__(self, parent):
        QtGui.QDialog.__init__(self, parent)
        self.ui = Ui_ConnectDialog()
        self.ui.setupUi(self)
        self.connect(self.ui.addserver_button, core.SIGNAL("clicked()"), 
                self, core.SLOT("addServerClicked()"))
        self.connect(self.ui.buttonBox.button(QtGui.QDialogButtonBox.Ok), 
                core.SIGNAL("clicked()"), self, core.SLOT("memcache_connect()"))
        self.ui.scrolling_contents.setLayout(QtGui.QVBoxLayout())
        self.ui.scrolling_contents.layout().setContentsMargins(0, 0, 0, 0)

    def addServerClicked(self):
        self.addHostEntry()

    def memcache_connect(self):
        mc.mc_connect(&servers)

    def retab(self):
        last = QtGui.QWidget()
        QWidget.setTabOrder(m_ui.buttonBox, m_ui.addserver_button)
        last = self.ui.addserver_button

        for i in servers:
            QWidget.setTabOrder(last, i.host)
            QWidget.setTabOrder(i.host, i.port)
            last = i.port
        QWidget.setTabOrder(last, self.ui.buttonBox)

    def addHostEntry(self):
        server_info_horiz_layout = QtGui.QHBoxLayout()
        host_label = QtGui.QLabel()
        host_port_spacer = QtGui.QSpacerItem()
        port_label = QtGui.QLabel()
        host_widget = QtGui.QWidget()
        host = HostEntry()

        m_ui.scrolling_contents.layout().addWidget(host_widget)
        host = HostEntry()
        server_info_horiz_layout = QtGui.QHBoxLayout(host_widget)
        server_info_horiz_layout.setObjectName("server_info_horiz_layout %d" % (servers.count()))
        server_info_horiz_layout.setContentsMargins(0, 0, 0, 0)
        host_label = new QLabel(host_widget)
        host_label.setObjectName("host_label %1" % (servers.count()))
        host_label.setText("Host %1: " % (servers.count()+1))

        server_info_horiz_layout.addWidget(host_label)

        host.host = QtGui.QLineEdit(host_widget)
        host.host.setObjectName("host_edit %1" % (servers.count()))

        server_info_horiz_layout.addWidget(host.host)

        host_port_spacer = QtGui.QSpacerItem(28, 20, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Minimum)

        server_info_horiz_layout.addItem(host_port_spacer)

        port_label = new QtGui.QLabel(host_widget)
        port_label.setObjectName("port_label %1" % (servers.count()))
        port_label.setText("Port %1: " % (servers.count()+1))

        server_info_horiz_layout.addWidget(port_label)

        host.port = QtGui.QLineEdit(host_widget)
        host.port.setObjectName("port_edit %1" % (servers.count()))
        host.port.setEnabled(true)
        QSizePolicy sizePolicy1(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(host.port.sizePolicy().hasHeightForWidth())
        host.port.setSizePolicy(sizePolicy1)
        host.port.setMaximumSize(QtGui.QSize(70, 16777215))
        host.port.setMaxLength(5)

        host.port.setValidator(new QtGui.QRegExpValidator(host.port_regex, self))

        server_info_horiz_layout.addWidget(host.port)
        host.host.setFocus()
        servers.append(host)

        self.retab()

    def __del__(self):
        for h in servers:
            del h
        del ui

    def changeEvent(e)
        if e.type() == QtGui.QEvent.LanguageChange:
            self.ui.retranslateUi(self)
