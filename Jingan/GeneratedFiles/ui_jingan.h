/********************************************************************************
** Form generated from reading UI file 'jingan.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JINGAN_H
#define UI_JINGAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JinganClass
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *JinganClass)
    {
        if (JinganClass->objectName().isEmpty())
            JinganClass->setObjectName(QStringLiteral("JinganClass"));
        JinganClass->resize(600, 400);
        actionExit = new QAction(JinganClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(JinganClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        JinganClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(JinganClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        JinganClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(JinganClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        JinganClass->addToolBar(Qt::BottomToolBarArea, mainToolBar);
        statusBar = new QStatusBar(JinganClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        JinganClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(JinganClass);

        QMetaObject::connectSlotsByName(JinganClass);
    } // setupUi

    void retranslateUi(QMainWindow *JinganClass)
    {
        JinganClass->setWindowTitle(QApplication::translate("JinganClass", "Jingan", 0));
        actionExit->setText(QApplication::translate("JinganClass", "Exit", 0));
        menuFile->setTitle(QApplication::translate("JinganClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class JinganClass: public Ui_JinganClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JINGAN_H
