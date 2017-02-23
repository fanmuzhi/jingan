#include "JinganInfo.h"
#include "Syn_Test_Utils.h"

JinganInfo::JinganInfo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	Qt::WindowFlags flags = Qt::Dialog;
	this->setWindowFlags(flags);

	this->SetInfo();

	QObject::connect(ui.OKPushButton, SIGNAL(clicked()), this, SLOT(close()));
}

JinganInfo::~JinganInfo()
{

}

void JinganInfo::SetInfo()
{
	ui.InfoNameLabel->setText("Huangpu++");

	ui.InfoVersionLabel->setText(QString("Version:") + SW_VERSION);

	ui.InfoDateLabel->setText(QString("Release Date:") + SW_RELEASEDATE);

	ui.InfoCopyrightLabel->setText("Copyright@2017 Synaptics");
}