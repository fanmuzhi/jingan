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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JinganClass
{
public:
    QAction *actionExit;
    QAction *actionLocalSettings;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTableWidget *TestEngineTableWidget;
    QPushButton *OperationPushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOption;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *JinganClass)
    {
        if (JinganClass->objectName().isEmpty())
            JinganClass->setObjectName(QStringLiteral("JinganClass"));
        JinganClass->resize(571, 609);
        actionExit = new QAction(JinganClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionLocalSettings = new QAction(JinganClass);
        actionLocalSettings->setObjectName(QStringLiteral("actionLocalSettings"));
        centralWidget = new QWidget(JinganClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        TestEngineTableWidget = new QTableWidget(centralWidget);
        if (TestEngineTableWidget->rowCount() < 9)
            TestEngineTableWidget->setRowCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        TestEngineTableWidget->setVerticalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        TestEngineTableWidget->setVerticalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        TestEngineTableWidget->setVerticalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        TestEngineTableWidget->setVerticalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        TestEngineTableWidget->setVerticalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        TestEngineTableWidget->setVerticalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        TestEngineTableWidget->setVerticalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        TestEngineTableWidget->setVerticalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        TestEngineTableWidget->setVerticalHeaderItem(8, __qtablewidgetitem8);
        TestEngineTableWidget->setObjectName(QStringLiteral("TestEngineTableWidget"));
        TestEngineTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout->addWidget(TestEngineTableWidget, 0, 0, 1, 1);

        OperationPushButton = new QPushButton(centralWidget);
        OperationPushButton->setObjectName(QStringLiteral("OperationPushButton"));

        gridLayout->addWidget(OperationPushButton, 1, 0, 1, 1);

        JinganClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(JinganClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 571, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuOption = new QMenu(menuBar);
        menuOption->setObjectName(QStringLiteral("menuOption"));
        JinganClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(JinganClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        JinganClass->addToolBar(Qt::BottomToolBarArea, mainToolBar);
        statusBar = new QStatusBar(JinganClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        JinganClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOption->menuAction());
        menuFile->addAction(actionExit);
        menuOption->addAction(actionLocalSettings);

        retranslateUi(JinganClass);

        QMetaObject::connectSlotsByName(JinganClass);
    } // setupUi

    void retranslateUi(QMainWindow *JinganClass)
    {
        JinganClass->setWindowTitle(QApplication::translate("JinganClass", "Jingan", 0));
        actionExit->setText(QApplication::translate("JinganClass", "Exit", 0));
        actionLocalSettings->setText(QApplication::translate("JinganClass", "LocalSettings", 0));
        QTableWidgetItem *___qtablewidgetitem = TestEngineTableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("JinganClass", "TestEngine", 0));
        QTableWidgetItem *___qtablewidgetitem1 = TestEngineTableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("JinganClass", "Status", 0));
        QTableWidgetItem *___qtablewidgetitem2 = TestEngineTableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("JinganClass", "SerialNumber", 0));
        QTableWidgetItem *___qtablewidgetitem3 = TestEngineTableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("JinganClass", "SNR", 0));
        QTableWidgetItem *___qtablewidgetitem4 = TestEngineTableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("JinganClass", "Bincodes", 0));
        QTableWidgetItem *___qtablewidgetitem5 = TestEngineTableWidget->verticalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("JinganClass", "Pass/Fail", 0));
        QTableWidgetItem *___qtablewidgetitem6 = TestEngineTableWidget->verticalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("JinganClass", "Image(NoFinger)", 0));
        QTableWidgetItem *___qtablewidgetitem7 = TestEngineTableWidget->verticalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("JinganClass", "Image(FakeFinger)", 0));
        QTableWidgetItem *___qtablewidgetitem8 = TestEngineTableWidget->verticalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("JinganClass", "TestStep", 0));
        OperationPushButton->setText(QApplication::translate("JinganClass", "Run", 0));
        menuFile->setTitle(QApplication::translate("JinganClass", "File", 0));
        menuOption->setTitle(QApplication::translate("JinganClass", "Option", 0));
    } // retranslateUi

};

namespace Ui {
    class JinganClass: public Ui_JinganClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JINGAN_H
