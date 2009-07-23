#include "memcacheclient.h"
#include "hostentry.h"
#include "singlesocket.h"
#include "StatData.h"
#include "GetData.h"
#include "macros.h"
#include "AppController.h"
#include <QVector>
#include <QAbstractSocket>
#include <QDataStream>
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QByteArray>

MemcacheClient::MemcacheClient(AppController *owner) : QObject(), owner(owner)
{
	lastCommand = NONE_CMD;
	responses = 0;
}

void MemcacheClient::mc_connect(QVector<HostEntry *> &hosts)
{
	HostEntry *h;
	SingleSocket *s;
	int id;

	this->hosts = &hosts;

	cleanAll();

	foreach (h, hosts) {
		id = connections.size();
		s = new SingleSocket(id, this);

		connect(s, SIGNAL(readyRead()), this, SLOT(readData()));
		connect(s, SIGNAL(error(QAbstractSocket::SocketError)),
				this, SLOT(socketError(QAbstractSocket::SocketError)));

		s->connectToHost(h->host->text(), h->port->text().toInt());
		connections.append(s);
	}
	qDebug("Connected");

	stats.resize(connections.size());
	get.resize(connections.size());
	data.resize(connections.size());

	/* populate the main window with some data */
	getStats();
}

void MemcacheClient::cleanAll()
{
	qDeleteAll(stats);
	qDeleteAll(connections);
	qDeleteAll(get);

	connections.clear();
	stats.clear();
	data.clear();
}

void MemcacheClient::getStats()
{
	sendCommandToAll("stats\r\n", STATS_CMD);
}

void MemcacheClient::flushAll()
{
	sendCommandToAll("flush_all\r\n", FLUSH_CMD);
}

void MemcacheClient::getItem(QString key)
{
	sendCommandToAll(QString("get %1\r\n").arg(key).toAscii(), RET_CMD);
}

void MemcacheClient::sendCommandToAll(const char *command, const CommandType cmd_num)
{
	QTcpSocket *c;

	lastCommand = cmd_num;
	responses = 0;

	qDeleteAll(stats);
	qDeleteAll(get);

	foreach (c, connections) {
		c->write(command);
	}

	owner->setBusy(true);
}

/**
 * This function needs to accept differnt types of data (rather than just a
 * string) and to add the item to only one of the servers, not all, preferably
 * using a user-selectable hashing algorithm (ketema, python mod, etc.)
 *
 * It would also be pretty slick we offered the ability to serialize the
 * sent string in as in PHP, Python, Java, etc.
 */
void MemcacheClient::addItem(QString UNUSED(key), QString UNUSED(data))
{
	return;
	QTcpSocket *c;

	lastCommand = STORE_CMD;

	foreach (c, connections) {
		c->write("");
	}
}

/**
 * stateful storage of data (QVector<QString>) should be a list of 'Response'
 * objects and response needs to be checked for errors
 */
void MemcacheClient::readData()
{
	QByteArray data_block;
	char *s;
	int len, sockid;
	SingleSocket *c;

	sockid = dynamic_cast<SingleSocket *>(sender())->id();
	c = connections.at(sockid);
	QDataStream in(c);


	qDebug("Called readData(%d)", sockid);
	forever {
		len = c->bytesAvailable();

		if (len == 0) {
			break;
		}

		s = new char[len];
		for (int i = 0; i < len+1; i++) s[i] = 0;

		in.readRawData(s, len);
		data_block.append(s);
		delete[] s;
	}

	data[sockid] = data_block;

	if (countResponses())
		handleResponse();
}

/**
  * XXX
  * there is no error checking on responses
  */
void MemcacheClient::handleResponse()
{
	int i;

	owner->setBusy(false);

	qDeleteAll(stats);
	qDeleteAll(get);

	for (i = 0; i < data.size(); i++) {
		switch (lastCommand) {
			case STATS_CMD:
				// clear out old stats
				stats[i] = new StatData(
						hosts->at(i)->host->text().append(":").append(
								hosts->at(i)->port->text()
						),
						QString(data[i])
				);
				break;
			case RET_CMD:
				get[i] = new GetData(
						hosts->at(i)->host->text().append(":").append(
								hosts->at(i)->port->text()
						),
						data[i]
				);
				break;
			case RET_CMD:

				break;
			default:
				break;
		}
	}

	switch (lastCommand) {
		case STATS_CMD:
			emit hasNewStats(stats);
			break;
		case RET_CMD:
			emit hasNewGet(get);
			break;
	}
}

bool MemcacheClient::countResponses()
{
	if (++responses == connections.size()) {
		responses = 0;
		return true;
	} else return false;
}

void MemcacheClient::socketError(QAbstractSocket::SocketError err)
{
	switch (err) {
		case QAbstractSocket::RemoteHostClosedError:
			owner->alert(tr("Memcache Inspector"), tr("The connection to the "
						"server has been terminated")
			);
			break;
		case QAbstractSocket::HostNotFoundError:
			owner->alert(tr("Memcache Inspector"), tr("The host was not found. "
						"Please check the host name and port settings.")
			);
			break;
		case QAbstractSocket::ConnectionRefusedError:
			owner->alert(tr("Memcache Inspector"), tr("The connection was "
						"refused by the peer. Make sure the memcache server "
						"is running, and check that the host name and port "
						"settings are correct.")
			);
			break;
		default:
			owner->alert(tr("Memcache Inspector"), tr("An Unknown error "
						"occurred.")
			);
	}
}
