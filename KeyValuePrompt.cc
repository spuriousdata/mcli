#include "KeyValuePrompt.h"
#include "ui_KeyValuePrompt.h"

KeyValuePrompt::KeyValuePrompt(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::KeyValuePrompt)
{
	ui->setupUi(this);
}

void KeyValuePrompt::setKey(const QString& key)
{
	ui->key->setText(key);
}

void KeyValuePrompt::setValue(const QString& value)
{
	ui->value->setPlainText(value);
}

QString KeyValuePrompt::key() const
{
	return ui->key->text();
}

QString KeyValuePrompt::value() const
{
	return ui->value->toPlainText();
}

QDialogButtonBox *KeyValuePrompt::buttonBox() const
{
	return ui->buttonBox;
}

KeyValuePrompt::~KeyValuePrompt()
{
	delete ui;
}

void KeyValuePrompt::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
