#ifndef MCI_H
#define MCI_H

#include <QtGui/QMainWindow>
#include "connect_dialog.h"
#include "ConfigDialog.h"

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
    void addClicked();
    void deleteClicked();
    void getClicked();
    void flushallClicked();
    void displayStats();

private:
    Ui::McIClass *ui;
    ConnectDialog *connect_dialog;
    ConfigDialog *config_dialog;
};

#endif // MCI_H
