/********************************************************************************
** Form generated from reading UI file 'DeviceManagment.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICEMANAGMENT_H
#define UI_DEVICEMANAGMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceManagment
{
public:
    QGridLayout *gridLayout;
    QLabel *DeviceManagmentLabel;
    QTableWidget *DeviceManagmentTableWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *DeviceManagment)
    {
        if (DeviceManagment->objectName().isEmpty())
            DeviceManagment->setObjectName(QStringLiteral("DeviceManagment"));
        DeviceManagment->setWindowModality(Qt::ApplicationModal);
        DeviceManagment->resize(213, 287);
        gridLayout = new QGridLayout(DeviceManagment);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        DeviceManagmentLabel = new QLabel(DeviceManagment);
        DeviceManagmentLabel->setObjectName(QStringLiteral("DeviceManagmentLabel"));

        gridLayout->addWidget(DeviceManagmentLabel, 0, 0, 1, 1);

        DeviceManagmentTableWidget = new QTableWidget(DeviceManagment);
        if (DeviceManagmentTableWidget->columnCount() < 1)
            DeviceManagmentTableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        DeviceManagmentTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        DeviceManagmentTableWidget->setObjectName(QStringLiteral("DeviceManagmentTableWidget"));
        DeviceManagmentTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout->addWidget(DeviceManagmentTableWidget, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(DeviceManagment);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);


        retranslateUi(DeviceManagment);

        QMetaObject::connectSlotsByName(DeviceManagment);
    } // setupUi

    void retranslateUi(QWidget *DeviceManagment)
    {
        DeviceManagment->setWindowTitle(QApplication::translate("DeviceManagment", "DeviceManagment", 0));
        DeviceManagmentLabel->setText(QApplication::translate("DeviceManagment", "Select Device for TestEngine", 0));
        QTableWidgetItem *___qtablewidgetitem = DeviceManagmentTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DeviceManagment", "Device SerialNumber", 0));
    } // retranslateUi

};

namespace Ui {
    class DeviceManagment: public Ui_DeviceManagment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICEMANAGMENT_H
