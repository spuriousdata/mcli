#include "GetData.h"
#include "GetItem.h"
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QByteArrayMatcher>
#include <QVector>

GetData::GetData(QString& host, QByteArray& data) : server(host)
{
	QString key, line, cas, flags;
	QByteArrayMatcher lineMatcher("\r\n", 2);
	QByteArrayMatcher endMatcher("END\r\n", 5);
	QStringList lineParts;
	QByteArray item;
	unsigned bytes, pos = 0;

	/* while index of END\r\n != 0 (end of message) */
	while (endMatcher.indexIn(data, pos) != 0) {
		pos = lineMatcher.indexIn(data);
		line = data.left(pos);
		pos += 2; // skip \r\n
		data = data.right(data.length() - pos);

		lineParts = line.split(' ');
		if (lineParts.length() == 5) {
			cas = lineParts[4].toULongLong();
		} else if (lineParts.length() != 4) {
			qDebug() << "WTF? " << lineParts;
		}

		key = lineParts[1];
		flags = lineParts[2];
		bytes = lineParts[3].toULong();

		item = data.left(bytes);
		data = data.right(data.length() - (bytes + 2)); // 2 more for \r\n again
		pos = 0;

		items.append(
				new GetItem(
						key,
						flags,
						cas,
						item
				)
		);
	}
}

GetData::~GetData()
{
	qDeleteAll(items);
}
