#include "mci.h"
#include "UIController.h"
#include "ConfigDialog.h"
#include "ConnectDialog.h"
#include "KeyValuePrompt.h"
#include "BusyIndicator.h"
#include "DialogState.h"
#include "StatData.h"
#include "ui_mci.h"
#include "AppController.h"
#include <QSettings>
#include <QMessageBox>

UIController::UIController(AppController *owner) : QObject(), owner(owner)
{
	connectDialog = NULL;
	configDialog = NULL;
	kvPrompt = NULL;
	busy = NULL;

	state = new DialogState();
	mciDialog = new McI();
	parent = mciDialog;

	/* openConfigDialog */
	connect(mciDialog->ui()->action_properties, SIGNAL(triggered()),
			this, SLOT(openConfigDialog()));

	/* openConnectDialog */
	connect(mciDialog->ui()->action_quick_connect, SIGNAL(triggered()),
			this, SLOT(openConnectDialog()));
	connect(mciDialog->ui()->connect_button, SIGNAL(clicked()),
			this, SLOT(openConnectDialog()));

	/* promptForAdd */
	connect(mciDialog->ui()->add_button, SIGNAL(clicked()),
			this, SLOT(promptForAdd()));

	/* promptForGet */
	connect(mciDialog->ui()->get_button, SIGNAL(clicked()),
			this, SLOT(promptForGet()));

	/* promptForDelete */
	connect(mciDialog->ui()->delete_button, SIGNAL(clicked()),
			this, SLOT(promptForDelete()));

	/* doFlushAll */
	connect(mciDialog->ui()->flushall_button, SIGNAL(clicked()),
			this, SIGNAL(doFlushAll()));

	/* doGetStats */
	connect(mciDialog->ui()->getstats_button, SIGNAL(clicked()),
			this, SIGNAL(doGetStats()));

	/* connect stats handler to hasNewStats signal */
	connect(this, SIGNAL(hasNewStats(QVector<StatData*>&)), mciDialog, SLOT(displayStats(QVector<StatData *>&)));
}

void UIController::setBusy(bool isBusy)
{
	if (isBusy) {
		if (!busy) {
			busy = new BusyIndicator(mciDialog);
		}
		busy->show();
		busy->raise();
		busy->activateWindow();
	} else {
		busy->hide();
	}
	mciDialog->setInputEnabled(!isBusy);
}

void UIController::alert(QString& title, QString& body)
{
	QMessageBox::information(mciDialog, title, body);
}

void UIController::openConfigDialog()
{
	if (!configDialog) {
		configDialog = new ConfigDialog(parent);
		connect(configDialog, SIGNAL(configDone()), this, SIGNAL(doConfigDone()));
	}

	configDialog->show();
	configDialog->raise();
	configDialog->activateWindow();
}

void UIController::openConnectDialog()
{

	if (!connectDialog) {
		QSettings settings(owner->settingsOrg(), owner->settingsName());

		connectDialog = new ConnectDialog(parent);

		settings.beginGroup("Connection");
		connectDialog->setSavedServers(settings.value("servers").toStringList());
		settings.endGroup();
	}

	/* emit a doConnect() signal when the connectDialog's connectReady() signal is emitted */
	connect(connectDialog, SIGNAL(connectReady()), this, SIGNAL(doConnect()));



	connectDialog->show();
	connectDialog->raise();
	connectDialog->activateWindow();
}

void UIController::showMainWindow()
{
	mciDialog->show();
}

void UIController::openKeyValuePrompt(
		const QString& title,
		const QString& keyStateKey,
		const QString& valueStateKey,
		const char *okSlot,
		bool showValueField
)
{
	if (!kvPrompt) {
		kvPrompt = new KeyValuePrompt(parent);
	}

	kvPrompt->setWindowTitle(title);
	kvPrompt->setKey(state->get(keyStateKey).toString());
	kvPrompt->showValueField(showValueField);
	kvPrompt->setValue(state->get(valueStateKey).toString());

	disconnect(kvPrompt->buttonBox()->button(QDialogButtonBox::Ok), SIGNAL(clicked()));
	connect(kvPrompt->buttonBox()->button(QDialogButtonBox::Ok),
			SIGNAL(clicked()),
			this,
			okSlot
	);

	kvPrompt->show();
	kvPrompt->raise();
	kvPrompt->activateWindow();
}

void UIController::promptForAdd()
{
	openKeyValuePrompt("Add Item Dialog", "AddItemDialog/lastKey", "AddItemDialog/lastValue", SIGNAL(doAdd()), true);
}

void UIController::promptForDelete()
{
	openKeyValuePrompt("Delete Item Dialog", "DeleteItemDialog/lastKey", "", SIGNAL(doDelete()));
}

void UIController::promptForGet()
{
	openKeyValuePrompt("Get Item Dialog", "GetItemDialog/lastKey", "", SIGNAL(doGet()));
}
