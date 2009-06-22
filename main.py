from PyQt4 import QtGui as qt
from mci import Mci
import sys


def main():
    a = qt.QApplication(sys.argv)
    w = Mci(a)
    w.show()
    sys.exit(a.exec_())


if __name__ == '__main__':
    main()
