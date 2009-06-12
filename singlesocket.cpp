#include "singlesocket.h"
#include <QTcpSocket>
#include <QObject>

SingleSocket::SingleSocket(int id, QObject *parent) : QTcpSocket(parent)
{
    socketid = id;
    connect(this, SIGNAL(readyRead()), this, SLOT(emitReadyRead()));
}

void SingleSocket::emitReadyRead()
{
    emit _readyRead(socketid);
}
