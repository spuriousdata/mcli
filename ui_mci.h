/********************************************************************************
** Form generated from reading ui file 'mci.ui'
**
** Created: Wed Jun 10 21:49:10 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MCI_H
#define UI_MCI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_McIClass
{
public:
    QAction *action_quick_connect;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QTreeView *ltree;
    QTreeView *rtree;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *add_button;
    QPushButton *delete_button;
    QPushButton *get_button;
    QPushButton *flushall_button;
    QMenuBar *menuBar;
    QMenu *menuEdit;
    QMenu *menu_Edit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *McIClass)
    {
        if (McIClass->objectName().isEmpty())
            McIClass->setObjectName(QString::fromUtf8("McIClass"));
        McIClass->resize(602, 421);
        action_quick_connect = new QAction(McIClass);
        action_quick_connect->setObjectName(QString::fromUtf8("action_quick_connect"));
        centralWidget = new QWidget(McIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        ltree = new QTreeView(splitter);
        ltree->setObjectName(QString::fromUtf8("ltree"));
        splitter->addWidget(ltree);
        rtree = new QTreeView(splitter);
        rtree->setObjectName(QString::fromUtf8("rtree"));
        splitter->addWidget(rtree);

        verticalLayout->addWidget(splitter);

        verticalSpacer = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        add_button = new QPushButton(centralWidget);
        add_button->setObjectName(QString::fromUtf8("add_button"));

        horizontalLayout->addWidget(add_button);

        delete_button = new QPushButton(centralWidget);
        delete_button->setObjectName(QString::fromUtf8("delete_button"));

        horizontalLayout->addWidget(delete_button);

        get_button = new QPushButton(centralWidget);
        get_button->setObjectName(QString::fromUtf8("get_button"));

        horizontalLayout->addWidget(get_button);

        flushall_button = new QPushButton(centralWidget);
        flushall_button->setObjectName(QString::fromUtf8("flushall_button"));

        horizontalLayout->addWidget(flushall_button);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);

        McIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(McIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 602, 22));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menu_Edit = new QMenu(menuBar);
        menu_Edit->setObjectName(QString::fromUtf8("menu_Edit"));
        McIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(McIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        McIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(McIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        McIClass->setStatusBar(statusBar);

        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menu_Edit->menuAction());
        menuEdit->addAction(action_quick_connect);

        retranslateUi(McIClass);

        QMetaObject::connectSlotsByName(McIClass);
    } // setupUi

    void retranslateUi(QMainWindow *McIClass)
    {
        McIClass->setWindowTitle(QApplication::translate("McIClass", "McI", 0, QApplication::UnicodeUTF8));
        action_quick_connect->setText(QApplication::translate("McIClass", "&Quick Connect", 0, QApplication::UnicodeUTF8));
        add_button->setText(QApplication::translate("McIClass", "Add", 0, QApplication::UnicodeUTF8));
        delete_button->setText(QApplication::translate("McIClass", "Delete", 0, QApplication::UnicodeUTF8));
        get_button->setText(QApplication::translate("McIClass", "Get", 0, QApplication::UnicodeUTF8));
        flushall_button->setText(QApplication::translate("McIClass", "Flush All", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("McIClass", "&File", 0, QApplication::UnicodeUTF8));
        menu_Edit->setTitle(QApplication::translate("McIClass", "&Edit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class McIClass: public Ui_McIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MCI_H
