#include <QtGui/QApplication>
#include "mci.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    McI w;
    w.show();
    return a.exec();
}
