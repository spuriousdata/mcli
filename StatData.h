#ifndef STATDATA_H
#define STATDATA_H

#include <QString>
#include <QStringList>

class StatData
{
public:
	StatData(const QString& host, const QString& data);
	QString server;
	QStringList stats;
};

#endif // STATDATA_H
