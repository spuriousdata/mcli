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
	Q_DISABLE_COPY(MemcacheClient)
public:
	MemcacheClient();
	void mc_connect(QVector<HostEntry *> &hosts);
	void addItem(QString& key, QString& data);
	QVector<SingleSocket *> connections;
	QVector<StatData *> stats;

signals:
	void hasNewStats(QVector<StatData *>&);

private slots:
	void readData();
	void socketError(QAbstractSocket::SocketError err);

private:
	QVector<HostEntry *> *hosts;
	void getStats();
	enum CommandState {
		NONE,
		STATS,
		GET,
		SET,
		DELETE,
		FLUSH
	} lastCommand;
};

#endif // MEMCACHECLIENT_H
