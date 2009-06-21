from PyQt4 import QtGui as qt
from mci import Mci
from Config import Configuration
from getopt import getopt, GetoptError
import sys

config = None

def main():
    global config
    config = Configuration()
    a = qt.QApplication(sys.argv)
    w = Mci(a)
    w.show()
    sys.exit(a.exec_())


if __name__ == '__main__':
    main()
