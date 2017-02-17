#include "DeviceManagment.h"
#include <QtWidgets>
#include <QDialog>
#include <QPushButton>

DeviceManagment::DeviceManagment(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	
	ui.DeviceManagmentTableWidget->setColumnWidth(1, 320);
	ui.DeviceManagmentTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.DeviceManagmentTableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.DeviceManagmentTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

	QObject::connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(ButtonBoxClicked(QAbstractButton *)));
}

DeviceManagment::~DeviceManagment()
{
}

void DeviceManagment::showEvent(QShowEvent * event)
{
	QDialog::showEvent(event);
	LocalSettings *pLocalSettingsDlg = static_cast<LocalSettings*>(parentWidget());
	if (NULL != pLocalSettingsDlg)
	{
		//get row index
		int iSelectRowIndex = pLocalSettingsDlg->GetEngineRowIndex();
		if (iSelectRowIndex < 0)
			iSelectRowIndex = 0;
		ui.DeviceManagmentLabel->setText(QString("Select Device for TestEngine ") + QString::number(iSelectRowIndex + 1) + QString(":"));

		//get serial number list
		std::vector<std::string> listOfSerialNumber;
		pLocalSettingsDlg->GetSerialNumberList(listOfSerialNumber);

		ui.DeviceManagmentTableWidget->setRowCount(listOfSerialNumber.size());
		for (size_t i = 1; i <= listOfSerialNumber.size(); i++)
		{
			ui.DeviceManagmentTableWidget->setItem(i - 1, 0, new QTableWidgetItem(QString::fromStdString(listOfSerialNumber[i - 1])));
		}
	}
}

void DeviceManagment::ButtonBoxClicked(QAbstractButton * button)
{
	if (button == (ui.buttonBox->button(QDialogButtonBox::Ok)))
	{
		bool focus = ui.DeviceManagmentTableWidget->isItemSelected(ui.DeviceManagmentTableWidget->currentItem());
		if (!focus)
		{
			QMessageBox::critical(this, QString("Error"), QString("Select a Device for TestEngine, please!"));
			return;
		}

		LocalSettings *pLocalSettingsDlg = static_cast<LocalSettings*>(parentWidget());
		if (NULL != pLocalSettingsDlg)
		{
			int iSerialNumberRowIndex = ui.DeviceManagmentTableWidget->currentRow();
			QString strSerialNumber = ui.DeviceManagmentTableWidget->item(iSerialNumberRowIndex, 0)->text();

			pLocalSettingsDlg->SelectDevice(strSerialNumber);

			this->close();
		}
	}
	else
	{
		this->close();
	}
}
