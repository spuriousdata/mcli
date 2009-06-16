from PyQt4 import QtGui as qt, QtCore as core
from ui_mci import Ui_MciClass
from connect_dialog import connect_dialog

class Mci(qt.QMainWindow):
    def __init__(self, parent):
        qt.QMainWindow.__init__(self)
        self.ui = Ui_MciClass()
        self.ui.setupUi(self)

        self.connect(self.ui.action_quick_connect, core.SIGNAL(triggered()), self, core.SLOT(self.openConnectDialog()))
        self.connect(self.ui.add_button, core.SIGNAL(clicked()), self, core.SLOT(self.addClicked()))
        self.connect(self.ui.delete_button, core.SIGNAL(clicked()), self, core.SLOT(self.deleteClicked()))
        self.connect(self.ui.get_button, core.SIGNAL(clicked()), self, core.SLOT(self.getClicked()))
        self.connect(self.ui.flushall_button, core.SIGNAL(clicked()), self, core.SLOT(self.flushallClicked()))

    def openConnectDialog()
        if self.connect_dialog == None:
            self.connect_dialog = ConnectDialog(self)
    
        connect_dialog->show()
        connect_dialog->raise()
        connect_dialog->activateWindow()

        self.connect(self.connect_dialog.mc, core.SIGNAL(hasNewStats()), self, core.SLOT(displayStats()))
}

void McI::displayStats()
{

}

void McI::addClicked()
{
    qDebug("addClicked() called")
}

void McI::getClicked()
{
    qDebug("getClicked() called")
}

void McI::deleteClicked()
{
    qDebug("deleteClicked() called")
}
void McI::flushallClicked()
{
    qDebug("flushallClicked() called")
}

McI::~McI()
{
    delete ui
    delete connect_dialog
}
