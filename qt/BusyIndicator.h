#ifndef BUSYINDICATOR_H
#define BUSYINDICATOR_H

#include <QtGui/QDialog>

namespace Ui {
    class BusyIndicator;
}

class BusyIndicator : public QDialog {
    Q_OBJECT
public:
    BusyIndicator(QWidget *parent = 0);
    ~BusyIndicator();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::BusyIndicator *m_ui;
};

#endif // BUSYINDICATOR_H
