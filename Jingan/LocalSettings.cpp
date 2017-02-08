#include "LocalSettings.h"
#include "DeviceManagment.h"

#include <QMessageBox>
#include <QFileDialog>

LocalSettings::LocalSettings(QWidget *parent)
: QDialog(parent)
, _pDeviceManager(NULL)
{
	ui.setupUi(this);

	ui.TestEngineTableWidget->verticalHeader()->setVisible(false);
	ui.TestEngineTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.TestEngineTableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.TestEngineTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

	//slots
	QObject::connect(ui.ConfigFilePushButton, SIGNAL(clicked()), this, SLOT(SelectConfigFile()));
	QObject::connect(ui.LogFilePathPushButton, SIGNAL(clicked()), this, SLOT(SelectLogFilePath()));
	QObject::connect(ui.TestEngineNumberSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ModifyTestEngineCounts(int)));
	QObject::connect(ui.ModifySerialNumberPushButton, SIGNAL(clicked()), this, SLOT(CreateDeviceManagment()));

	QObject::connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(ButtonBoxClicked(QAbstractButton *)));

	//init
	this->Init();
}

LocalSettings::~LocalSettings()
{
	if (NULL != _pDeviceManager)
	{
		delete _pDeviceManager;
		_pDeviceManager = NULL;
	}
}

void LocalSettings::Init()
{
	uint32_t rc = 0;

	LocalSettingsConfig *pSynLocalSettingConfig = NULL;
	LocalSettingsConfig::CreateLSConfigInstance(LocalSettingsConfig::Read, pSynLocalSettingConfig);
	if (NULL == pSynLocalSettingConfig)
	{
		LocalSettingsConfig::CreateLSConfigInstance(LocalSettingsConfig::Save, pSynLocalSettingConfig);
	}
	pSynLocalSettingConfig->GetLocalSettings(_LocalSettingsConfig);
	delete pSynLocalSettingConfig;
	pSynLocalSettingConfig = NULL;

	//operation
	ui.ConfigFileLineEdit->clear();
	ui.ConfigFileLineEdit->setText(_LocalSettingsConfig.strConfigFilePath);

	ui.LogFilePathLineEdit->clear();
	ui.LogFilePathLineEdit->setText(_LocalSettingsConfig.strLogFilePath);

	unsigned int DeviceCounts = _LocalSettingsConfig.list_serialnumber.size();
	ui.TestEngineNumberSpinBox->setValue(DeviceCounts);

	ui.TestEngineTableWidget->setRowCount(DeviceCounts);
	for (size_t i = 0; i < DeviceCounts; i++)
	{
		QString strTestEngineIndex(QString::number(i + 1));
		QString strDeviceSerialNumber(QString::fromStdString((_LocalSettingsConfig.list_serialnumber)[i]));

		ui.TestEngineTableWidget->setItem(i, 0, new QTableWidgetItem(strTestEngineIndex));
		ui.TestEngineTableWidget->setItem(i, 1, new QTableWidgetItem(strDeviceSerialNumber));
	}

	if (NULL == _pDeviceManager)
	{
		_pDeviceManager = new syn_devicemanager();
	}
	QString strDeviceType = QString::fromStdString(_LocalSettingsConfig.strDeviceType);
	devicetype Type;
	if ("MPC04" == strDeviceType.toUpper())
	{
		Type = spi_mpc04;
		ui.DeviceTypeComboBox->setCurrentIndex(1);
	}
	else
	{
		Type = spi_m5;
	}
	_pDeviceManager->Connect(Type);
}

int LocalSettings::GetEngineRowIndex()
{
	return ui.TestEngineTableWidget->currentRow();
}

void LocalSettings::GetSerialNumberList(vector<string> &ListOfSerialNumber)
{
	ListOfSerialNumber.clear();

	if (NULL == _pDeviceManager)
	{
		return;
	}

	devicetype Type;
	QString strDeviceType = ui.DeviceTypeComboBox->itemText(ui.DeviceTypeComboBox->currentIndex());
	if ("MPC04" == strDeviceType.toUpper())
		Type = spi_mpc04;
	else
		Type = spi_m5;

	_pDeviceManager->Connect(Type);
	_pDeviceManager->GetSerialNumberList(ListOfSerialNumber);
	if (0 == ListOfSerialNumber.size())
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't find device,check it please!"));
		return;
	}
}

void LocalSettings::SelectDevice(const QString strSerialNumber)
{
	int iSiteRowIndex = ui.TestEngineTableWidget->currentRow();
	if (iSiteRowIndex < 0)
		iSiteRowIndex = 0;
	ui.TestEngineTableWidget->setItem(iSiteRowIndex, 1, new QTableWidgetItem(strSerialNumber));
}

void LocalSettings::SelectConfigFile()
{
	QString strConfigFilePath = QFileDialog::getOpenFileName(this, "Select Config File", "", "xml file(*.xml)");

	QFile qConfigFile(strConfigFilePath);
	if (!qConfigFile.exists())
		return;

	ui.ConfigFileLineEdit->clear();
	ui.ConfigFileLineEdit->setText(strConfigFilePath);

	_LocalSettingsConfig.strConfigFilePath = strConfigFilePath;
}

void LocalSettings::SelectLogFilePath()
{
	QString strLogFolderPath = QFileDialog::getExistingDirectory(this, "Select LogFile Path");
	QFile qFile(strLogFolderPath);
	if (!qFile.exists())
		return;
	
	ui.LogFilePathLineEdit->clear();
	ui.LogFilePathLineEdit->setText(strLogFolderPath);

	_LocalSettingsConfig.strLogFilePath = strLogFolderPath;
}

void LocalSettings::ModifyTestEngineCounts(int i)
{
	if (NULL == _pDeviceManager)
	{
		return;
	}

	int EngineCounts = ui.TestEngineNumberSpinBox->value();

	devicetype Type;
	QString strDeviceType = ui.DeviceTypeComboBox->itemText(ui.DeviceTypeComboBox->currentIndex());
	if ("MPC04" == strDeviceType.toUpper())
		Type = spi_mpc04;
	else
		Type = spi_m5;

	_pDeviceManager->Connect(Type);
	vector<string> ListOfSerialNumber;
	_pDeviceManager->GetSerialNumberList(ListOfSerialNumber);
	if (EngineCounts > ListOfSerialNumber.size())
	{
		QMessageBox::critical(this, QString("Error"), QString("Current Engine numbers is more than real Device conunts(") + QString::number(ListOfSerialNumber.size()) + QString(")!"));
		return;
	}

	int iCurrentRowCounts = ui.TestEngineTableWidget->rowCount();
	if (iCurrentRowCounts < EngineCounts)
	{
		for (unsigned int i = iCurrentRowCounts + 1; i <= EngineCounts; i++)
		{
			ui.TestEngineTableWidget->insertRow(i - 1);
			ui.TestEngineTableWidget->setItem(i - 1, 0, new QTableWidgetItem(QString::number(i)));
			ui.TestEngineTableWidget->setItem(i - 1, 1, new QTableWidgetItem(QString("0")));
		}
	}
	else if (iCurrentRowCounts > EngineCounts)
	{
		for (unsigned int i = iCurrentRowCounts; i > EngineCounts; i--)
		{
			ui.TestEngineTableWidget->removeRow(i - 1);
		}
	}
}

void LocalSettings::CreateDeviceManagment()
{
	DeviceManagment *pDeviceManagment = new DeviceManagment(this);
	pDeviceManagment->setAttribute(Qt::WA_DeleteOnClose);
	pDeviceManagment->show();
}

void LocalSettings::ButtonBoxClicked(QAbstractButton * button)
{
	if (button == (ui.buttonBox->button(QDialogButtonBox::Ok)))
	{
		this->Confirm();
	}
	else
	{
		this->close();
	}
}

void LocalSettings::Confirm()
{
	uint32_t rc(0);

	//check info first
	int iUserEngineCounts = ui.TestEngineTableWidget->rowCount();
	if (0 == iUserEngineCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Current Engine counts is 0!"));
		return;
	}

	vector<string> ListOfCurrentSerialNumber;
	for (int i = 0; i < iUserEngineCounts; i++)
	{
		QString strSerialNumber("");
		QTableWidgetItem *pSerialNumberItem = ui.TestEngineTableWidget->item(i, 1);
		if (NULL != pSerialNumberItem)
			strSerialNumber = pSerialNumberItem->text();

		if (0 == strSerialNumber.size())
		{
			QMessageBox::critical(this, QString("Error"), QString("Engine ") + QString::number(i + 1) + QString("'s SerialNumber is 0!"));
			return;
		}

		bool IsExists(false);
		for (int j = 1; j <= ListOfCurrentSerialNumber.size(); j++)
		{
			if (strSerialNumber.toStdString() == ListOfCurrentSerialNumber[j - 1])
			{
				IsExists = true;
				break;
			}
		}
		if (IsExists)
		{
			QMessageBox::critical(this, QString("Error"), QString("Engine ") + QString::number(i + 1) + QString("'s SerialNumber has been used!"));
			return;
		}

		ListOfCurrentSerialNumber.push_back(strSerialNumber.toStdString());
	}

	//update LocalSettings info
	QString strConfigFilePath = ui.ConfigFileLineEdit->text();
	QFile qConfigfile(strConfigFilePath);
	if (!qConfigfile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("Config file is not exists,check it please!"));
		return;
	}
	_LocalSettingsConfig.strConfigFilePath = strConfigFilePath;

	QString strLogFilePath = ui.LogFilePathLineEdit->text();
	QFile qLogfile(strLogFilePath);
	if (!qLogfile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("LogFile Path is not exists,check it please!"));
		return;
	}
	_LocalSettingsConfig.strLogFilePath = strLogFilePath;

	QString strDeviceType = ui.DeviceTypeComboBox->itemText(ui.DeviceTypeComboBox->currentIndex());
	_LocalSettingsConfig.strDeviceType = strDeviceType.toStdString();

	//TestEngine info
	_LocalSettingsConfig.list_serialnumber.clear();
	for (int i = 0; i < iUserEngineCounts; i++)
	{
		QString strSerialNumber("");
		QTableWidgetItem *pSerialNumberItem = ui.TestEngineTableWidget->item(i, 1);
		if (NULL != pSerialNumberItem)
			strSerialNumber = pSerialNumberItem->text();

		_LocalSettingsConfig.list_serialnumber.push_back(strSerialNumber.toStdString());
	}

	//save
	LocalSettingsConfig *pLocalSettingsInstance = NULL;
	rc = LocalSettingsConfig::CreateLSConfigInstance(LocalSettingsConfig::Save, pLocalSettingsInstance);
	if (NULL != pLocalSettingsInstance)
	{
		pLocalSettingsInstance->SetLocalSettings(_LocalSettingsConfig);
		delete pLocalSettingsInstance;
		pLocalSettingsInstance = NULL;
	}

	this->close();
}
