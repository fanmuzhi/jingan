/********************************************************************************
** Form generated from reading UI file 'JinganInfo.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JINGANINFO_H
#define UI_JINGANINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JinganInfo
{
public:
    QFormLayout *formLayout;
    QLabel *LogoLabel;
    QFrame *frame;
    QFormLayout *formLayout_2;
    QLabel *InfoNameLabel;
    QLabel *InfoVersionLabel;
    QLabel *InfoDateLabel;
    QLabel *InfoCopyrightLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *OKPushButton;

    void setupUi(QWidget *JinganInfo)
    {
        if (JinganInfo->objectName().isEmpty())
            JinganInfo->setObjectName(QStringLiteral("JinganInfo"));
        JinganInfo->setWindowModality(Qt::ApplicationModal);
        JinganInfo->resize(291, 137);
        formLayout = new QFormLayout(JinganInfo);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        LogoLabel = new QLabel(JinganInfo);
        LogoLabel->setObjectName(QStringLiteral("LogoLabel"));
        LogoLabel->setPixmap(QPixmap(QString::fromUtf8(":/Jingan/images/Synaptics.bmp")));

        formLayout->setWidget(0, QFormLayout::LabelRole, LogoLabel);

        frame = new QFrame(JinganInfo);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        formLayout_2 = new QFormLayout(frame);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        InfoNameLabel = new QLabel(frame);
        InfoNameLabel->setObjectName(QStringLiteral("InfoNameLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, InfoNameLabel);

        InfoVersionLabel = new QLabel(frame);
        InfoVersionLabel->setObjectName(QStringLiteral("InfoVersionLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, InfoVersionLabel);

        InfoDateLabel = new QLabel(frame);
        InfoDateLabel->setObjectName(QStringLiteral("InfoDateLabel"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, InfoDateLabel);

        InfoCopyrightLabel = new QLabel(frame);
        InfoCopyrightLabel->setObjectName(QStringLiteral("InfoCopyrightLabel"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, InfoCopyrightLabel);


        formLayout->setWidget(0, QFormLayout::FieldRole, frame);

        horizontalSpacer = new QSpacerItem(189, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(1, QFormLayout::LabelRole, horizontalSpacer);

        OKPushButton = new QPushButton(JinganInfo);
        OKPushButton->setObjectName(QStringLiteral("OKPushButton"));

        formLayout->setWidget(1, QFormLayout::FieldRole, OKPushButton);


        retranslateUi(JinganInfo);

        QMetaObject::connectSlotsByName(JinganInfo);
    } // setupUi

    void retranslateUi(QWidget *JinganInfo)
    {
        JinganInfo->setWindowTitle(QApplication::translate("JinganInfo", "About Huangpu++", 0));
        LogoLabel->setText(QString());
        InfoNameLabel->setText(QApplication::translate("JinganInfo", "Name", 0));
        InfoVersionLabel->setText(QApplication::translate("JinganInfo", "Version", 0));
        InfoDateLabel->setText(QApplication::translate("JinganInfo", "Date", 0));
        InfoCopyrightLabel->setText(QApplication::translate("JinganInfo", "Copyright", 0));
        OKPushButton->setText(QApplication::translate("JinganInfo", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class JinganInfo: public Ui_JinganInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JINGANINFO_H
