#include "StatData.h"
#include <QString>
#include <QStringList>

StatData::StatData(const QString& host, const QString& data)
{
	QStringList lines;
	QString line;

	server = host;
	if (data.contains("\r\n", Qt::CaseInsensitive)) {
		lines = data.split("\r\n");

		foreach (line, lines) {
			stats.append(line);
			if (line.startsWith("END")) break;
		}
	} else {
		qDebug(data.toAscii());
	}
}
