#include "mci.h"
#include "connect_dialog.h"
#include "ConfigDialog.h"
#include "ui_mci.h"

#include <QtGui>

class MemcacheClient;

McI::McI(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::McIClass)
{
    ui->setupUi(this);
    connect(ui->action_quick_connect, SIGNAL(triggered()), this, SLOT(openConnectDialog()));
    connect(ui->action_properties, SIGNAL(triggered()), this, SLOT(openConfigDialog()));
    connect(ui->add_button, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(ui->delete_button, SIGNAL(clicked()), this, SLOT(deleteClicked()));
    connect(ui->get_button, SIGNAL(clicked()), this, SLOT(getClicked()));
    connect(ui->flushall_button, SIGNAL(clicked()), this, SLOT(flushallClicked()));
    connect(ui->connect_button, SIGNAL(clicked()), this, SLOT(openConnectDialog()));
}

void McI::openConfigDialog()
{
    if (!config_dialog) {
        config_dialog = new ConfigDialog(this);
    }
    config_dialog->show();
    config_dialog->raise();
    config_dialog->activateWindow();
}

void McI::openConnectDialog()
{
    if (!connect_dialog) {
        connect_dialog = new ConnectDialog(this);
    }
    connect_dialog->show();
    connect_dialog->raise();
    connect_dialog->activateWindow();
}

void McI::displayStats()
{
    QTreeWidget *tree;
    QTreeWidgetItem *parent, *host, *pair;
    QStringList data;
    QString line;
    bool hasHost = false;

    tree = ui->maintree;

    tree->setHeaderLabels(QStringList() << tr("Host") << tr("Key") << tr("value"));
    tree->header()->setResizeMode(0, QHeaderView::Stretch);
    tree->header()->setResizeMode(1, QHeaderView::Stretch);
    tree->header()->setResizeMode(2, QHeaderView::Stretch);

    tree->clear();
    parent = tree->invisibleRootItem();

    data = connect_dialog->mc->stats;

    host = new QTreeWidgetItem(parent);
    host->setText(0, "Test Host 1");
    pair = new QTreeWidgetItem(host);
    pair->setText(0, "mem");
    pair->setText(1, "12345");

    pair = new QTreeWidgetItem(host);
    pair->setText(0, "foo");
    pair->setText(1, "asdfasdf");

    pair = new QTreeWidgetItem(host);
    pair->setText(0, "bar");
    pair->setText(1, "1qwereqw2345");

    pair = new QTreeWidgetItem(host);
    pair->setText(0, "baz");
    pair->setText(1, "rturtu");

    /*

    foreach (line, data) {
        if (!hasHost) {
            host = new QTreeWidgetItem(parent);
            hasHost = true;
            host->setText(0, line);
        } else {
            pair = new QTreeWidgetItem(host);
            QStringList parts = line.split(' ');
            if (parts.size() == 3) {
                pair->setText(0, parts.at(1));
                pair->setText(1, parts.at(2));
            } else {
                pair->setText(0, line);
            }
        }
    }
*/
    tree->sortByColumn(0);
    tree->setFocus();
}

void McI::addClicked()
{
    qDebug("addClicked() called");
}

void McI::getClicked()
{
    qDebug("getClicked() called");
}

void McI::deleteClicked()
{
    qDebug("deleteClicked() called");
}
void McI::flushallClicked()
{
    qDebug("flushallClicked() called");
}

McI::~McI()
{
    delete ui;
    delete connect_dialog;
}
