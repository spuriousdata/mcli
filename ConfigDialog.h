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
    virtual ~ConfigDialog();

public slots:
    void toggle_unpw(int val);

private:
    Ui::ConfigDialog *ui;

};

#endif	/* _CONFIG_H */

