#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QtGui/QDialog>
#include <QVector>
#include "hostentry.h"
#include "memcacheclient.h"

namespace Ui {
	class ConnectDialog;
}

class ConnectDialog : public QDialog {
	Q_OBJECT
	Q_DISABLE_COPY(ConnectDialog)

public:
	explicit ConnectDialog(QWidget *parent = 0);
	virtual ~ConnectDialog();
	QVector<HostEntry *> servers;

public slots:
	void addServerClicked();

signals:
	void connectReady();

protected:
	virtual void changeEvent(QEvent *e);

private:
	Ui::ConnectDialog *m_ui;
	void addHostEntry();
	void retab();
};

#endif // DIALOG_H
