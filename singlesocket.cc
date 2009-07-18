#include "singlesocket.h"
#include <QTcpSocket>
#include <QObject>

SingleSocket::SingleSocket(int id, QObject *parent) : QTcpSocket(parent)
{
	socketid = id;
}

int SingleSocket::id() const
{
	return socketid;
}
