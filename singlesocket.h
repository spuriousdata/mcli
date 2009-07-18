#ifndef SINGLESOCKET_H
#define SINGLESOCKET_H

#include <QTcpSocket>
#include <QObject>

class SingleSocket : public QTcpSocket
{
	Q_OBJECT
public:
	SingleSocket(int id, QObject *parent = 0);
	int id() const;

private:
	int socketid;
};

#endif // SINGLESOCKET_H
