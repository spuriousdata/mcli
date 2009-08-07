#include <QString>
#include <QByteArray>
#include "GetItem.h"


GetItem::GetItem(QString& k, QString& f, QString& c, QByteArray d)
{
	key = k;
	flags = f;
	cas_unique = c;
	data = d;
}
