#ifndef MEMCACHECLIENT_H
#define MEMCACHECLIENT_H

#include "hostentry.h"
#include <QObject>
#include <QVector>
#include <QAbstractSocket>

class SingleSocket;
class StatData;

class MemcacheClient : public QObject
{
	Q_OBJECT
public:
	MemcacheClient() {}
	void mc_connect(QVector<HostEntry *>* hosts);
	QVector<SingleSocket *> connections;
	QVector<StatData*> stats;

signals:
	void hasNewStats();

private slots:
	void readData(int sockid);
	void socketError(QAbstractSocket::SocketError err);

private:
	QVector<HostEntry *> *hosts;
	void getStats();
};

#endif // MEMCACHECLIENT_H
