#include "memcacheclient.h"
#include "hostentry.h"
#include "singlesocket.h"
#include <QVector>
#include <QAbstractSocket>
#include <QDataStream>
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>

void MemcacheClient::mc_connect(QVector<HostEntry *> *hosts)
{
    HostEntry *h;
    SingleSocket *s;
    int id;

    this->hosts = hosts;
    
    foreach (h, *hosts) {
        id = connections.size();
        s = new SingleSocket(id, this);
        connect(s, SIGNAL(_readyRead(int)), this, SLOT(readData(int)));
        connect(s, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));
        s->connectToHost(h->host->text(), h->port->text().toInt());
        connections.append(s);
    }
    qDebug("Connected");
    /* XXX */
    getStats();
}

void MemcacheClient::getStats()
{
    QTcpSocket *c;

    foreach (c, connections) {
        c->write("stats\r\n");
    }
}

void MemcacheClient::readData(int sockid)
{
    SingleSocket *c;
    c = connections.at(sockid);
    QDataStream in(c);
    QString data_block;
    char *s;
    int len;
    qDebug("Called readData(%d)", sockid);
    forever {
        len = c->bytesAvailable();

        if (len == 0) {
            break;
        }

        s = new char(len+1);
        in.readRawData(s, len);
        data_block.append(s);
        delete s;
    }

    // First entry is the hostname for now
    stats << QString(hosts->at(sockid)->host->text().append(":").append(hosts->at(sockid)->port->text()))  << data_block.split("\r\n");
    emit hasNewStats();

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
