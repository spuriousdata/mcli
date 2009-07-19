#include "memcacheclient.h"
#include "hostentry.h"
#include "singlesocket.h"
#include "StatData.h"
#include "macros.h"
#include <QVector>
#include <QAbstractSocket>
#include <QDataStream>
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>

MemcacheClient::MemcacheClient() : QObject(), lastCommand(NONE_CMD)
{
	/*
	QNetworkProxy proxy;
	proxy.setType(QNetworkProxy::Socks5Proxy);
	proxy.setHostName("proxy.example.com");
	proxy.setPort(1080);
	proxy.setUser("username");
	proxy.setPassword("password");
	QNetworkProxy::setApplicationProxy(proxy);
 */
}

void MemcacheClient::mc_connect(QVector<HostEntry *> &hosts)
{
	HostEntry *h;
	SingleSocket *s;
	int id;

	this->hosts = &hosts;

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

	/* populate the main window with some data */
	getStats();
}

void MemcacheClient::getStats()
{
	QTcpSocket *c;

	lastCommand = STATS_CMD;

	foreach (c, connections) {
		c->write("stats\r\n");
	}
}

/**
 * This function needs to accept differnt types of data
 *  and to add the item to only one of the servers, not all
 *
 *  It would also be pretty slick we offered the ability to serialize the
 *  sent string in as in PHP, Python, Java, etc.
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

void MemcacheClient::readData()
{
	QString data_block;
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

		s = new char[len+1]();
		for (int i = 0; i < len+1; i++) s[i] = 0;

		in.readRawData(s, len);
		data_block.append(s);
		delete[] s;
	}

	if (lastCommand == STATS_CMD) {
		// clear out old stats
		if (stats[sockid]) delete stats[sockid];

		stats[sockid] = new StatData(
				hosts->at(sockid)->host->text().append(":").append(hosts->at(sockid)->port->text()),
				data_block
		);

		emit hasNewStats(stats);
	}
}

void MemcacheClient::socketError(QAbstractSocket::SocketError err)
{
	switch (err) {
		case QAbstractSocket::RemoteHostClosedError:
			QMessageBox::information(NULL,
									 tr("Memcache Inspector"),
									 tr("The connection to the "
										"server has been terminated"));
			break;
		case QAbstractSocket::HostNotFoundError:
			QMessageBox::information(NULL,
									 tr("Memcache Inspector"),
									 tr("The host was not found. Please check "
										"the host name and port settings."));
			break;
		case QAbstractSocket::ConnectionRefusedError:
			QMessageBox::information(NULL,
									 tr("Memcache Inspector"),
									 tr("The connection was refused by the peer. "
										"Make sure the memcache server is running, "
										"and check that the host name and port "
										"settings are correct."));
			break;
		default:
			QMessageBox::information(NULL,
									tr("Memcache Inspector"),
									tr("The following error occurred:"
									   "."));
	}
}
