#ifndef GETDATA_H
#define GETDATA_H

#include <QString>
#include <QByteArray>
#include <QVector>

class GetItem;

class GetData
{
public:
	GetData(QString& host, QByteArray& data);
	~GetData();
	QString server;
	QVector<GetItem *> items;

};

#endif // GETDATA_H
