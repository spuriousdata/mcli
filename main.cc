#include <QtGui/QApplication>
#include "AppController.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AppController c;
	c.run();
	return a.exec();
}
