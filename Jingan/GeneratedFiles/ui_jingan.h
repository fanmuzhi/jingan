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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JinganClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *JinganClass)
    {
        if (JinganClass->objectName().isEmpty())
            JinganClass->setObjectName(QStringLiteral("JinganClass"));
        JinganClass->resize(600, 400);
        menuBar = new QMenuBar(JinganClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        JinganClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(JinganClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        JinganClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(JinganClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        JinganClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(JinganClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        JinganClass->setStatusBar(statusBar);

        retranslateUi(JinganClass);

        QMetaObject::connectSlotsByName(JinganClass);
    } // setupUi

    void retranslateUi(QMainWindow *JinganClass)
    {
        JinganClass->setWindowTitle(QApplication::translate("JinganClass", "Jingan", 0));
    } // retranslateUi

};

namespace Ui {
    class JinganClass: public Ui_JinganClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JINGAN_H
