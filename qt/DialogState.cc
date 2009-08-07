#include "DialogState.h"
#include <QString>
#include <QVariant>

void DialogState::set(QString& key, const QVariant& value)
{
	data[key] = value;
}

bool DialogState::exists(const QString& key) const
{
	return data.contains(key);
}

QVariant DialogState::get(const QString &key) const
{
	return data.value(key);
}
