#ifndef MCI_H
#define MCI_H

#include <QtGui/QMainWindow>
#include <QModelIndex>
#include "connect_dialog.h"
#include "ConfigDialog.h"
#include "KeyValuePrompt.h"

namespace Ui
{
	class McIClass;
}

class McI : public QMainWindow
{
	Q_OBJECT

public:
	McI(QWidget *parent = 0);
	~McI();

public slots:
	void openConnectDialog();
	void openConfigDialog();
	void openKeyValuePrompt();
	void addClicked();
	void do_add();
	void deleteClicked();
	void getClicked();
	void flushallClicked();
	void displayStats();
	void resizeTreeColumns(const QModelIndex& index);

private:
	Ui::McIClass *ui;
	ConnectDialog *connect_dialog;
	ConfigDialog *config_dialog;
	KeyValuePrompt *kv_prompt;
};

#endif // MCI_H
