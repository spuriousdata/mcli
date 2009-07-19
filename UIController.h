#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QObject>
#include "StatData.h"

class McI;
class ConnectDialog;
class ConfigDialog;
class KeyValuePrompt;
class DialogState;
class AppController;

class UIController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(UIController)

public:
	UIController(AppController *owner = 0);
	void showMainWindow();
	inline ConnectDialog *getConnectDialog() const {return connectDialog;}
	inline KeyValuePrompt *getKeyValuePrompt() const {return kvPrompt;}

public slots:
	void openConfigDialog();
	void openConnectDialog();
	void promptForAdd();
	void promptForGet();
	void promptForDelete();

signals:
	void doConnect();
	void doAdd();
	void doGet();
	void doDelete();
	void hasNewStats(QVector<StatData *>&);

private:
	McI *mciDialog;
	ConnectDialog *connectDialog;
	ConfigDialog *configDialog;
	KeyValuePrompt *kvPrompt;
	DialogState *state;
	QWidget *parent;
	AppController *owner;

	void openKeyValuePrompt(const QString& title, const QString& keyStateKey,
							const QString& valueStateKey, const char *okSlot,
							bool showValueField = false);
};

#endif // UICONTROLLER_H
