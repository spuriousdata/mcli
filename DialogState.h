#ifndef DIALOGSTATE_H
#define DIALOGSTATE_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QVariant>

class DialogState : public QObject
{
public:
	bool exists(const QString& key) const;
	void set(QString& key, const QVariant& value);
	QVariant get(const QString& key) const;

private:
	QHash<QString, QVariant> data;
};

#endif // DIALOGSTATE_H
