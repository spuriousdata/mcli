#ifndef KEYVALUEPROMPT_H
#define KEYVALUEPROMPT_H

#include <QtGui/QDialog>
#include <QDialogButtonBox>

namespace Ui {
	class KeyValuePrompt;
}

class KeyValuePrompt : public QDialog {
	Q_OBJECT
public:
	KeyValuePrompt(QWidget *parent = 0);
	~KeyValuePrompt();
	void setKey(const QString& key);
	void setValue(const QString& value);
	QString key() const;
	QString value() const;
	QDialogButtonBox *buttonBox() const;


protected:
	void changeEvent(QEvent *e);

private:
	Ui::KeyValuePrompt *ui;
};

#endif // KEYVALUEPROMPT_H
