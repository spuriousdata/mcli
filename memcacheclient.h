#ifndef MEMCACHECLIENT_H
#define MEMCACHECLIENT_H

#include "hostentry.h"
#include <QObject>
#include <QVector>
#include <QAbstractSocket>

class SingleSocket;
class StatData;
class AppController;

class MemcacheClient : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(MemcacheClient)
public:
	MemcacheClient(AppController *owner);
	void mc_connect(QVector<HostEntry *> &hosts);
	void addItem(QString key, QString data);
	QVector<SingleSocket *> connections;
	QVector<StatData *> stats;

public slots:
	void flushAll();

signals:
	void hasNewStats(QVector<StatData *>&);

private slots:
	void readData();
	void socketError(QAbstractSocket::SocketError err);

private:
	QVector<HostEntry *> *hosts;
	QVector<QString> data;
	AppController *owner;
	int responses;
	void getStats();
	void handleResponse();
	bool countResponses();
	enum CommandType {
		NONE_CMD,
		STORE_CMD,
		RET_CMD,
		DEL_CMD,
		INCDEC_CMD,
		STATS_CMD,
		FLUSH_CMD,
		VERSION_CMD
	} lastCommand;
	void sendCommandToAll(const char *command, const CommandType cmd_num);

};

#endif // MEMCACHECLIENT_H
