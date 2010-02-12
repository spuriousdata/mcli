#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QtGui/QDialog>
#include <QVector>
#include <QStringList>
#include "HostEntry.h"
#include "MemcacheClient.h"

namespace Ui {
	class ConnectDialog;
}

class ConnectDialog : public QDialog {
	Q_OBJECT
	Q_DISABLE_COPY(ConnectDialog)

public:
	explicit ConnectDialog(QWidget *parent = 0);
	virtual ~ConnectDialog();
	void setSavedServers(QStringList savedServers);
	QVector<HostEntry *> servers;

public slots:
	void addServerClicked();
	void removeServerClicked();

signals:
	void connectReady();

protected:
	virtual void changeEvent(QEvent *e);

private:
	Ui::ConnectDialog *m_ui;
	void addHostEntry(QString hostname = "", QString portnumber = "");
	void retab();
};

#endif // DIALOG_H
