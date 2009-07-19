#include "mci.h"
#include "UIController.h"
#include "ConfigDialog.h"
#include "ConnectDialog.h"
#include "KeyValuePrompt.h"
#include "DialogState.h"
#include "StatData.h"
#include "ui_mci.h"

UIController::UIController() : QObject()
{
	connectDialog = NULL;
	configDialog = NULL;
	kvPrompt = NULL;

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

	connect(this, SIGNAL(hasNewStats(QVector<StatData*>&)), mciDialog, SLOT(displayStats(QVector<StatData *>&)));
}

void UIController::openConfigDialog()
{
	if (!configDialog) {
		configDialog = new ConfigDialog(parent);
	}

	configDialog->show();
	configDialog->raise();
	configDialog->activateWindow();
}

void UIController::openConnectDialog()
{
	if (!connectDialog) {
		connectDialog = new ConnectDialog(parent);
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
