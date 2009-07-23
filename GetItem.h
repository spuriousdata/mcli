#ifndef GETITEM_H
#define GETITEM_H

#include <QString>
#include <QByteArray>

class GetItem
{
public:
	GetItem(QString& k, QString& f, QString& c, QByteArray d);
	QString key;
	QString flags;
	QString cas_unique;
	QByteArray data;
};

#endif // GETITEM_H
