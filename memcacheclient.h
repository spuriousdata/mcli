#ifndef MEMCACHECLIENT_H
#define MEMCACHECLIENT_H

#include "hostentry.h"
#include <QObject>
#include <QVector>
#include <QAbstractSocket>
class SingleSocket;
class QStringList;

class MemcacheClient : public QObject
{
    Q_OBJECT
public:
    MemcacheClient() {}
    void mc_connect(QVector<HostEntry *>* hosts);
    QStringList stats;

signals:
    void hasNewStats();

private slots:
    void readData(int sockid);
    void socketError(QAbstractSocket::SocketError err);

private:
    QVector<SingleSocket *> connections;
    QVector<HostEntry *> *hosts;
    void getStats();
};

#endif // MEMCACHECLIENT_H
