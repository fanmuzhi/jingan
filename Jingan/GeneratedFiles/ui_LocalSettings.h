/********************************************************************************
** Form generated from reading UI file 'LocalSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCALSETTINGS_H
#define UI_LOCALSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LocalSettings
{
public:
    QGridLayout *gridLayout_5;
    QGroupBox *DeviceTypeGroupBox;
    QGridLayout *gridLayout;
    QComboBox *DeviceTypeComboBox;
    QGroupBox *ConfigFileGroupBox;
    QGridLayout *gridLayout_2;
    QLineEdit *ConfigFileLineEdit;
    QPushButton *ConfigFilePushButton;
    QGroupBox *LogFilePathGroupBox;
    QGridLayout *gridLayout_3;
    QLineEdit *LogFilePathLineEdit;
    QPushButton *LogFilePathPushButton;
    QGroupBox *TestEngineGroupBox;
    QGridLayout *gridLayout_4;
    QLabel *TestEngineNumberLabel;
    QSpinBox *TestEngineNumberSpinBox;
    QTableWidget *TestEngineTableWidget;
    QPushButton *ModifySerialNumberPushButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *LocalSettings)
    {
        if (LocalSettings->objectName().isEmpty())
            LocalSettings->setObjectName(QStringLiteral("LocalSettings"));
        LocalSettings->setWindowModality(Qt::ApplicationModal);
        LocalSettings->resize(354, 516);
        gridLayout_5 = new QGridLayout(LocalSettings);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        DeviceTypeGroupBox = new QGroupBox(LocalSettings);
        DeviceTypeGroupBox->setObjectName(QStringLiteral("DeviceTypeGroupBox"));
        gridLayout = new QGridLayout(DeviceTypeGroupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        DeviceTypeComboBox = new QComboBox(DeviceTypeGroupBox);
        DeviceTypeComboBox->setObjectName(QStringLiteral("DeviceTypeComboBox"));

        gridLayout->addWidget(DeviceTypeComboBox, 0, 0, 1, 1);


        gridLayout_5->addWidget(DeviceTypeGroupBox, 0, 0, 1, 1);

        ConfigFileGroupBox = new QGroupBox(LocalSettings);
        ConfigFileGroupBox->setObjectName(QStringLiteral("ConfigFileGroupBox"));
        gridLayout_2 = new QGridLayout(ConfigFileGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        ConfigFileLineEdit = new QLineEdit(ConfigFileGroupBox);
        ConfigFileLineEdit->setObjectName(QStringLiteral("ConfigFileLineEdit"));
        ConfigFileLineEdit->setReadOnly(true);

        gridLayout_2->addWidget(ConfigFileLineEdit, 0, 0, 1, 1);

        ConfigFilePushButton = new QPushButton(ConfigFileGroupBox);
        ConfigFilePushButton->setObjectName(QStringLiteral("ConfigFilePushButton"));

        gridLayout_2->addWidget(ConfigFilePushButton, 0, 1, 1, 1);


        gridLayout_5->addWidget(ConfigFileGroupBox, 1, 0, 1, 1);

        LogFilePathGroupBox = new QGroupBox(LocalSettings);
        LogFilePathGroupBox->setObjectName(QStringLiteral("LogFilePathGroupBox"));
        gridLayout_3 = new QGridLayout(LogFilePathGroupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        LogFilePathLineEdit = new QLineEdit(LogFilePathGroupBox);
        LogFilePathLineEdit->setObjectName(QStringLiteral("LogFilePathLineEdit"));

        gridLayout_3->addWidget(LogFilePathLineEdit, 0, 0, 1, 1);

        LogFilePathPushButton = new QPushButton(LogFilePathGroupBox);
        LogFilePathPushButton->setObjectName(QStringLiteral("LogFilePathPushButton"));

        gridLayout_3->addWidget(LogFilePathPushButton, 0, 1, 1, 1);


        gridLayout_5->addWidget(LogFilePathGroupBox, 2, 0, 1, 1);

        TestEngineGroupBox = new QGroupBox(LocalSettings);
        TestEngineGroupBox->setObjectName(QStringLiteral("TestEngineGroupBox"));
        gridLayout_4 = new QGridLayout(TestEngineGroupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        TestEngineNumberLabel = new QLabel(TestEngineGroupBox);
        TestEngineNumberLabel->setObjectName(QStringLiteral("TestEngineNumberLabel"));

        gridLayout_4->addWidget(TestEngineNumberLabel, 0, 0, 1, 1);

        TestEngineNumberSpinBox = new QSpinBox(TestEngineGroupBox);
        TestEngineNumberSpinBox->setObjectName(QStringLiteral("TestEngineNumberSpinBox"));
        TestEngineNumberSpinBox->setMinimum(0);
        TestEngineNumberSpinBox->setMaximum(20);

        gridLayout_4->addWidget(TestEngineNumberSpinBox, 0, 1, 1, 1);

        TestEngineTableWidget = new QTableWidget(TestEngineGroupBox);
        if (TestEngineTableWidget->columnCount() < 2)
            TestEngineTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        TestEngineTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        TestEngineTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        TestEngineTableWidget->setObjectName(QStringLiteral("TestEngineTableWidget"));
        TestEngineTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_4->addWidget(TestEngineTableWidget, 1, 0, 1, 2);

        ModifySerialNumberPushButton = new QPushButton(TestEngineGroupBox);
        ModifySerialNumberPushButton->setObjectName(QStringLiteral("ModifySerialNumberPushButton"));

        gridLayout_4->addWidget(ModifySerialNumberPushButton, 2, 0, 1, 2);


        gridLayout_5->addWidget(TestEngineGroupBox, 3, 0, 1, 1);

        buttonBox = new QDialogButtonBox(LocalSettings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_5->addWidget(buttonBox, 4, 0, 1, 1);


        retranslateUi(LocalSettings);

        QMetaObject::connectSlotsByName(LocalSettings);
    } // setupUi

    void retranslateUi(QWidget *LocalSettings)
    {
        LocalSettings->setWindowTitle(QApplication::translate("LocalSettings", "LocalSettings", 0));
        DeviceTypeGroupBox->setTitle(QApplication::translate("LocalSettings", "Device Type", 0));
        DeviceTypeComboBox->clear();
        DeviceTypeComboBox->insertItems(0, QStringList()
         << QApplication::translate("LocalSettings", "M5", 0)
         << QApplication::translate("LocalSettings", "MPC04", 0)
        );
        ConfigFileGroupBox->setTitle(QApplication::translate("LocalSettings", "Config File", 0));
        ConfigFilePushButton->setText(QApplication::translate("LocalSettings", "Select", 0));
        LogFilePathGroupBox->setTitle(QApplication::translate("LocalSettings", "LogFile Path", 0));
        LogFilePathPushButton->setText(QApplication::translate("LocalSettings", "Select", 0));
        TestEngineGroupBox->setTitle(QApplication::translate("LocalSettings", "TestEngine", 0));
        TestEngineNumberLabel->setText(QApplication::translate("LocalSettings", "Engine Numbers:", 0));
        QTableWidgetItem *___qtablewidgetitem = TestEngineTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("LocalSettings", "Engine Index", 0));
        QTableWidgetItem *___qtablewidgetitem1 = TestEngineTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("LocalSettings", "Device SerialNumber", 0));
        ModifySerialNumberPushButton->setText(QApplication::translate("LocalSettings", "Modify SerialNumber", 0));
    } // retranslateUi

};

namespace Ui {
    class LocalSettings: public Ui_LocalSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCALSETTINGS_H
