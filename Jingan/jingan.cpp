#include "jingan.h"

#include <QMessageBox>
#include <QKeyEvent>

Jingan::Jingan(QWidget *parent)
: QMainWindow(parent)
, _finishedEngineCounts(0)
{
	ui.setupUi(this);
	ui.TestEngineTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.TestEngineTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	QObject::connect(ui.actionExit, SIGNAL(triggered(bool)), this, SLOT(Exit()));

	QObject::connect(ui.actionLocalSettings, SIGNAL(triggered(bool)), this, SLOT(CreateLocalSettings()));

	//Testing Operation
	//QObject::connect(ui.pushButtonRun, SIGNAL(clicked()), this, SLOT(Run()));


	this->Initialize();
}

Jingan::~Jingan()
{
	this->ClearTestEngines();
}

void Jingan::Exit()
{
	QCoreApplication::exit();
}

void Jingan::Initialize()
{
	uint32_t rc = 0;

	//LocalSettings
	LocalSettingsConfig *pSynLocalSettingConfig = NULL;
	rc = LocalSettingsConfig::CreateLSConfigInstance(LocalSettingsConfig::Read, pSynLocalSettingConfig);
	if (0 != rc || NULL == pSynLocalSettingConfig)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't retrieve TestEnigne info from LocalSettings, check it please!!"));
		return;
	}
	rc =  pSynLocalSettingConfig->GetLocalSettings(_LocalSettingConfig);
	delete pSynLocalSettingConfig;
	pSynLocalSettingConfig = NULL;

	unsigned int iLocalEngineCounts = _LocalSettingConfig.list_serialnumber.size();
	if (0 == iLocalEngineCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't retrieve TestEnigne info from LocalSettings(counts), check it please!!"));
		return;
	}

	//ConstructSiteList
	//Config file
	QString strConfigFilePath = _LocalSettingConfig.strConfigFilePath;
	QFile qConfigFile(strConfigFilePath);
	if (!qConfigFile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't find config file, check it please!"));
		return;
	}

	//clear
	this->ClearTestEngines();

	bool IsSucceed(true);
	for (size_t i = 0; i < iLocalEngineCounts; i++)
	{
		string strSerialNumber = _LocalSettingConfig.list_serialnumber[i];
		uint32_t EngineNumber = i + 1;
		Syn_TestEngine *pTestEngineInstance = NULL;
		string strConfigFilePath = _LocalSettingConfig.strConfigFilePath.toStdString();
		rc = Syn_TestEngine::CreateTestEngine(EngineNumber, strSerialNumber, strConfigFilePath, pTestEngineInstance);
		if (0 != rc || NULL == pTestEngineInstance)
		{
			QMessageBox::critical(this, QString("Error"), QString("Can't cosntruct SerialNumber:") + QString::fromStdString(strSerialNumber) + QString(" device") +  QString(", check it please!"));
			IsSucceed = false;
			break;
		}

		_ListOfTestEngine.push_back(pTestEngineInstance);
	}

	if (!IsSucceed)
	{
		this->ClearTestEngines();
		return;
	}
	size_t EngineCounts = _ListOfTestEngine.size();
	if (0 == EngineCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't construct the Engine list, check it please!"));
		return;
	}
	
	for (int t = ui.TestEngineTableWidget->columnCount(); t >= 1; t--)
	{
		ui.TestEngineTableWidget->removeColumn(t - 1);
	}

	ui.TestEngineTableWidget->setColumnCount(EngineCounts);
	QStringList strListOfHeader;
	for (size_t t = 1; t <= EngineCounts; t++)
	{
		uint32_t TestEngineNumber(0);
		_ListOfTestEngine[t - 1]->GetTestEngineNumber(TestEngineNumber);

		string strSerialNumber;
		_ListOfTestEngine[t - 1]->GetDeviceSerialNumber(strSerialNumber);
		
		Syn_TestEngine::EngineState EngineState =_ListOfTestEngine[t - 1]->GetStatus();
		QString strEngineStatus = TransformEngineStatus(EngineState);
		
		QTableWidgetItem *qItemSiteNumber = new QTableWidgetItem(QString::number(TestEngineNumber) + QString(" (") + QString::fromStdString(strSerialNumber) + QString(") "));
		QTableWidgetItem *qItemSiteStatus = new QTableWidgetItem(strEngineStatus);
		qItemSiteNumber->setTextAlignment(Qt::AlignCenter);
		qItemSiteStatus->setTextAlignment(Qt::AlignCenter);
		ui.TestEngineTableWidget->setItem(0, t - 1, qItemSiteNumber);
		ui.TestEngineTableWidget->setItem(1, t - 1, qItemSiteStatus);
		
		ui.TestEngineTableWidget->setColumnWidth(t - 1, 200);

		strListOfHeader.append(QString("#") + QString::number(TestEngineNumber));
	}
	ui.TestEngineTableWidget->setHorizontalHeaderLabels(strListOfHeader);
}

void Jingan::CreateLocalSettings()
{
	//clear
	this->ClearTestEngines();

	LocalSettings *_pLocalSettingsDlg = new LocalSettings(this);
	_pLocalSettingsDlg->show();
	_pLocalSettingsDlg->setAttribute(Qt::WA_DeleteOnClose);

	//connect LocalSettings close signal to re-construct site list
	QObject::connect(_pLocalSettingsDlg, SIGNAL(destroyed()), this, SLOT(Initialize()));
}

void Jingan::ClearTestEngines()
{
	if (0 != _ListOfTestEngine.size())
	{
		for (size_t i = _ListOfTestEngine.size(); i >= 1; i--)
		{
			delete _ListOfTestEngine[i - 1];
			_ListOfTestEngine[i - 1] = NULL;
		}
		_ListOfTestEngine.clear();
	}
}

void Jingan::keyPressEvent(QKeyEvent * ev)
{
	if (ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return)
	{
		//Run();
	}
}

QString Jingan::TransformEngineStatus(Syn_TestEngine::EngineState EngineState)
{
	QString strEngineStatus("");
	switch (EngineState)
	{
		case Syn_TestEngine::idle:
			strEngineStatus = "Idle";
			break;
		case Syn_TestEngine::data_ready:
			strEngineStatus = "Data Ready";
			break;
		case Syn_TestEngine::running:
			strEngineStatus = "Running";
			break;
		case Syn_TestEngine::error:
			strEngineStatus = "Error";
			break;
		case Syn_TestEngine::closed:
			strEngineStatus = "Closed";
			break;
		default:
			strEngineStatus = "Error";
			break;
	}

	return strEngineStatus;
}