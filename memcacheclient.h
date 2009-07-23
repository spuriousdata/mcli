#ifndef MEMCACHECLIENT_H
#define MEMCACHECLIENT_H

#include "hostentry.h"
#include <QObject>
#include <QVector>
#include <QAbstractSocket>
#include <QByteArray>

class SingleSocket;
class StatData;
class GetData;
class AppController;

class MemcacheClient : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(MemcacheClient)
public:
	MemcacheClient(AppController *owner);
	~MemcacheClient() {cleanAll();}
	void mc_connect(QVector<HostEntry *> &hosts);
	inline bool isConnected() { return (connections.size() > 0) ? true:false;}
	void addItem(QString key, QString data);
	void getItem(QString key);
	QVector<SingleSocket *> connections;
	QVector<StatData *> stats;
	QVector<GetData *> get;

public slots:
	void flushAll();
	void getStats();

signals:
	void hasNewStats(QVector<StatData *>&);
	void hasNewGet(QVector<GetData *>&);

private slots:
	void readData();
	void socketError(QAbstractSocket::SocketError err);

private:
	QVector<HostEntry *> *hosts;
	QVector<QByteArray> data;
	AppController *owner;
	int responses;
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
	void cleanAll();

};

#endif // MEMCACHECLIENT_H
