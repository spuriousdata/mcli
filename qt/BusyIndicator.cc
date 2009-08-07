#include "BusyIndicator.h"
#include "ui_BusyIndicator.h"

BusyIndicator::BusyIndicator(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::BusyIndicator)
{
    m_ui->setupUi(this);
}

BusyIndicator::~BusyIndicator()
{
    delete m_ui;
}

void BusyIndicator::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
