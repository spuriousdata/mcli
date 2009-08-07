#ifndef CONFIGDIALOG_H
#define	CONFIGDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
	class ConfigDialog;
}

class ConfigDialog : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY(ConfigDialog)

public:
	ConfigDialog(QWidget *parent = 0);
	const Ui::ConfigDialog *ui() const {return m_ui;}
	void setUseSocks(bool f);
	void setSocksDns(bool f);
	void setSocksHost(QString host);
	void setSocksPort(QString port);
	void setSocksUser(QString user);
	void setSocksPass(QString pass);
	virtual ~ConfigDialog();

signals:
	void configDone();

private:
	Ui::ConfigDialog *m_ui;

};

#endif	/* _CONFIG_H */

