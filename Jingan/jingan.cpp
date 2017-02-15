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
	QObject::connect(ui.OperationPushButton, SIGNAL(clicked()), this, SLOT(Run()));

	//Thread
	for (int i = 1; i <= TESTENGINE_COUNTS_MAX; i++)
	{
		QObject::connect(&(_qThreadArray[i - 1]), SIGNAL(sendTestStep(unsigned int, const QString, const QString)),
			this, SLOT(ReceiveTestStep(unsigned int, const QString, const QString)), Qt::ConnectionType(Qt::QueuedConnection));

		QObject::connect(&(_qThreadArray[i - 1]), SIGNAL(sendTestData(unsigned int, const dut_test_result *)),
			this, SLOT(ReceiveTestResults(unsigned int, const dut_test_result *)), Qt::ConnectionType(Qt::QueuedConnection));

		QObject::connect(&(_qImagingThreadArray[i - 1]), SIGNAL(sendImage(unsigned int, const dut_test_result *)),
			this, SLOT(ReceivingImage(unsigned int, const dut_test_result *)), Qt::ConnectionType(Qt::QueuedConnection));
	}

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

	for (size_t i = 0; i < EngineCounts; i++)
	{
		_qThreadArray[i].SetTestEngine(_ListOfTestEngine[i]);
		_qThreadArray[i].SetStopTag(true);

		_qImagingThreadArray[i].SetTestEngine(_ListOfTestEngine[i]);
		_qImagingThreadArray[i].SetStopTag(true);
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

	LocalSettings *pLocalSettingsDlg = new LocalSettings(this);
	pLocalSettingsDlg->show();
	pLocalSettingsDlg->setAttribute(Qt::WA_DeleteOnClose);

	//connect LocalSettings close signal to re-construct site list
	QObject::connect(pLocalSettingsDlg, SIGNAL(destroyed()), this, SLOT(Initialize()));
}


void Jingan::Run()
{
	unsigned int EngineCounts = _ListOfTestEngine.size();
	if (0 == EngineCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Engine numbers is 0,check it please!"));
		return;
	}

	_finishedEngineCounts = 0;

	FlagType flagType;

	QString qText = ui.OperationPushButton->text();
	if (QString("Run") == qText)
	{
		flagType = All;
		vector<string> list_teststep;
		_ListOfTestEngine[0]->GetTestStepList(list_teststep);
		for (size_t t = 0; t < list_teststep.size(); t++)
		{
			if ("WaitStimulus" == list_teststep[t])
			{
				flagType = Init;
				break;
			}
		}

		for (int i = 1; i <= EngineCounts; i++)
		{
			if (NULL != ui.TestEngineTableWidget->item(1, i - 1))
				ui.TestEngineTableWidget->takeItem(1, i - 1);
			if (NULL != ui.TestEngineTableWidget->item(2, i - 1))
				ui.TestEngineTableWidget->takeItem(2, i - 1);
			if (NULL != ui.TestEngineTableWidget->item(3, i - 1))
				ui.TestEngineTableWidget->takeItem(3, i - 1);
			if (NULL != ui.TestEngineTableWidget->item(4, i - 1))
				ui.TestEngineTableWidget->takeItem(4, i - 1);
			if (NULL != ui.TestEngineTableWidget->item(5, i - 1))
				ui.TestEngineTableWidget->takeItem(5, i - 1);
			if (NULL != ui.TestEngineTableWidget->cellWidget(6, i - 1))
				ui.TestEngineTableWidget->removeCellWidget(6, i - 1);
			if (NULL != ui.TestEngineTableWidget->cellWidget(7, i - 1))
				ui.TestEngineTableWidget->removeCellWidget(7, i - 1);
			if (NULL != ui.TestEngineTableWidget->item(8, i - 1))
				ui.TestEngineTableWidget->takeItem(8, i - 1);
		}

		ui.OperationPushButton->setDisabled(true);
		ui.actionLocalSettings->setDisabled(true);
	}
	else if (QString("Continue") == qText)
	{
		flagType = Final;

		ui.OperationPushButton->setDisabled(true);
		ui.actionLocalSettings->setDisabled(true);
	}

	for (int i = 1; i <= EngineCounts; i++)
	{
		if (_qImagingThreadArray[i - 1].isRunning())
		{
			_qImagingThreadArray[i - 1].SetStopTag(true);
			_qImagingThreadArray[i - 1].wait();
		}

		if (!_qThreadArray[i - 1].isRunning())
		{
			_qThreadArray[i - 1].start();
			_qThreadArray[i - 1].SetStopTag(false);
			_qThreadArray[i - 1].SetFlagType(flagType);
		}

		Syn_TestEngine::EngineState EngineStatus = _ListOfTestEngine[i - 1]->GetStatus();
		if (Syn_TestEngine::error == EngineStatus)
		{
			_finishedEngineCounts += 1;
		}
	}
}

void Jingan::ReceiveTestStep(unsigned int EngineNumber, const QString strTestStep, const QString strPassOrFail)
{
	unsigned int iPos = EngineNumber - 1;

	QString strPassOrFailResult = strPassOrFail;
	//SerialNumber
	if (QString("InitializationStep") == strTestStep)
	{
		QString strSensorSerialNumber = strPassOrFail.mid(5);
		strPassOrFailResult = strPassOrFail.mid(0, 4);
		if ("pass" == strPassOrFailResult.toLower())
		{
			//State
			QTableWidgetItem *itemSensorSerialNumber = new QTableWidgetItem(strSensorSerialNumber);
			itemSensorSerialNumber->setTextAlignment(Qt::AlignCenter);
			ui.TestEngineTableWidget->setItem(2, iPos, itemSensorSerialNumber);
		}
		else
			strPassOrFailResult = strPassOrFail;
	}

	//Display Results first
	QString qsStepAndResult = strTestStep + QString(" : ") + strPassOrFailResult;
	if (NULL != ui.TestEngineTableWidget->item(8, iPos))
	{
		QString qsTempContent = ui.TestEngineTableWidget->item(8, iPos)->text();
		ui.TestEngineTableWidget->item(8, iPos)->setText(qsTempContent + QString("\n") + qsStepAndResult);
		ui.TestEngineTableWidget->resizeRowToContents(8);
	}
	else
	{
		QTableWidgetItem *item = new QTableWidgetItem(qsStepAndResult);
		item->setTextAlignment(Qt::AlignCenter);
		ui.TestEngineTableWidget->setItem(8, iPos, item);
		ui.TestEngineTableWidget->resizeRowToContents(8);
	}

	if (QString("pass") != strPassOrFailResult.toLower())
	{
		ui.TestEngineTableWidget->item(8, iPos)->setBackgroundColor(QColor(255, 0, 0));
	}

	Syn_TestEngine::EngineState EngineStatus = _ListOfTestEngine[iPos]->GetStatus();
	if (Syn_TestEngine::error == EngineStatus)
	{
		//State
		QTableWidgetItem *itemState = new QTableWidgetItem(QString("Error"));
		itemState->setTextAlignment(Qt::AlignCenter);
		ui.TestEngineTableWidget->setItem(1, iPos, itemState);
		itemState->setBackgroundColor(QColor(255, 0, 0));
		return;
	}

	//State
	QTableWidgetItem *itemState = new QTableWidgetItem(QString("Running"));
	itemState->setTextAlignment(Qt::AlignCenter);
	ui.TestEngineTableWidget->setItem(1, iPos, itemState);	
}

void Jingan::ReceiveTestResults(unsigned int EngineNumber, const dut_test_result *pTestData)
{
	unsigned int iPos = EngineNumber - 1;

	FlagType flagType;
	bool bWaitStimulus(false);
	std::vector<std::string> listOfTestStepName;
	_ListOfTestEngine[iPos]->GetTestStepList(listOfTestStepName);
	if (0 != listOfTestStepName.size())
	{
		for (size_t i = 0; i < listOfTestStepName.size(); i++)
		{
			if (std::string("WaitStimulus") == listOfTestStepName[i])
			{
				bWaitStimulus = true;
				break;
			}
		}
	}
	if (bWaitStimulus)
	{
		QString qText = ui.OperationPushButton->text();
		if (QString("Run") == qText)
		{
			flagType = Init;
		}
		else if (QString("Continue") == qText)
		{
			flagType = Final;
		}
	}
	else
	{
		flagType = All;
	}

	_finishedEngineCounts += 1;

	Syn_TestEngine::EngineState Status = _ListOfTestEngine[iPos]->GetStatus();
	if (Syn_TestEngine::error == Status)
	{
		//State
		QTableWidgetItem *itemState = new QTableWidgetItem(QString("Error"));
		itemState->setTextAlignment(Qt::AlignCenter);
		ui.TestEngineTableWidget->setItem(1, iPos, itemState);
		itemState->setBackgroundColor(QColor(255, 0, 0));
		//this->ManageButtonStatus(iFlag);
		return;
	}

	//int rowNumber = CurrentSysConfig._uiNumRows;
	//int columnNumber = CurrentSysConfig._uiNumCols;

	if (Final == flagType || All == flagType)
	{
		//writelog
		_ListOfTestEngine[iPos]->WriteLog(_LocalSettingConfig.strLogFilePath.toStdString());

		//State
		QTableWidgetItem *itemState = new QTableWidgetItem(QString("Closed"));
		itemState->setTextAlignment(Qt::AlignCenter);
		ui.TestEngineTableWidget->setItem(1, iPos, itemState);

		QString strTestResult("");
		if (1 == pTestData->list_bincodes.size() && "1" == (pTestData->list_bincodes)[0])
			strTestResult = "Pass";
		else
			strTestResult = "Fail";
		QTableWidgetItem *itemTestResult = new QTableWidgetItem(strTestResult);
		if ("Pass" == strTestResult)
			itemTestResult->setBackgroundColor(QColor(0, 255, 0));
		else
			itemTestResult->setBackgroundColor(QColor(255, 0, 0));

		itemTestResult->setTextAlignment(Qt::AlignCenter);
		ui.TestEngineTableWidget->setItem(5, iPos, itemTestResult);
		
		//BinCode
		QString strBincodes("");
		for (size_t i = 1; i <= pTestData->list_bincodes.size(); i++)
		{
			if (1 == i)
				strBincodes += QString::fromStdString(pTestData->list_bincodes[i - 1]);
			else
				strBincodes = strBincodes + QString(" , ") + QString::fromStdString(pTestData->list_bincodes[i - 1]);
		}
		QTableWidgetItem *itemBincodes = new QTableWidgetItem(strBincodes);
		itemBincodes->setTextAlignment(Qt::AlignCenter);
		ui.TestEngineTableWidget->setItem(4, iPos, itemBincodes);

		_ListOfTestEngine[iPos]->Close();
	}

	this->ManageButtonStatus(flagType);
}

void Jingan::ManageButtonStatus(FlagType flag)
{
	if (_finishedEngineCounts == _ListOfTestEngine.size())
	{
		bool bAllFailed(true);
		for (size_t i = 1; i <= _ListOfTestEngine.size(); i++)
		{
			Syn_TestEngine::EngineState Status = _ListOfTestEngine[i - 1]->GetStatus();
			if (Syn_TestEngine::error != Status)
			{
				bAllFailed = false;
				break;
			}
		}

		if (bAllFailed)
		{

			//if all failed,reset it to init
			ui.OperationPushButton->setText(QString("Run"));
			ui.OperationPushButton->setDisabled(false);
			//ui.actionBinCodes->setDisabled(false);
			ui.actionLocalSettings->setDisabled(false);
		}
		else
		{
			if (Init == flag)
			{
				ui.OperationPushButton->setText(QString("Continue"));
				ui.OperationPushButton->setDisabled(false);

				//DisplayImage In time
				//DisplayImage In time
				for (size_t i = 1; i <= _ListOfTestEngine.size(); i++)
				{
					if (!_qImagingThreadArray[i - 1].isRunning())
					{
						_qImagingThreadArray[i - 1].SetStopTag(false);
						_qImagingThreadArray[i - 1].start();
					}
				}
			}
			else if (Final == flag)
			{
				ui.OperationPushButton->setText(QString("Run"));
				ui.OperationPushButton->setDisabled(false);
				//ui.actionBinCodes->setDisabled(false);
				ui.actionLocalSettings->setDisabled(false);
			}
			else
			{
				ui.OperationPushButton->setText(QString("Run"));
				ui.OperationPushButton->setDisabled(false);
				//ui.actionBinCodes->setDisabled(false);
				ui.actionLocalSettings->setDisabled(false);
			}
		}
	}
}

void Jingan::ReceivingImage(unsigned int EngineNumber, const dut_test_result *pTestData)
{
	unsigned int iPos = EngineNumber - 1;
	if (NULL == pTestData)
		return;

	bool CalibrateExcuted(false);
	for (size_t t = 0; t < pTestData->list_testdata.size(); t++)
	{
		SynTestData *Test_data = pTestData->list_testdata[t];
		if (NULL != Test_data)
		{
			if ("Calibrate" == Test_data->data_name)
			{
				CalibrateExcuted = true;
				break;
			}
			
		}
	}
	if (!CalibrateExcuted)
		return;

	WaitStilumusTestData *waitStilimusdata = NULL;
	for (size_t t = 0; t < pTestData->list_testdata.size(); t++)
	{
		SynTestData *Test_data = pTestData->list_testdata[t];
		if (NULL != Test_data)
		{
			if ("WaitStimulus" == Test_data->data_name)
			{
				waitStilimusdata = static_cast<WaitStilumusTestData*>(Test_data);
				break;
			}
		}
	}

	uint32_t rowNumber = 144;
	uint32_t columnNumber = 56;
	uint8_t *arrImage8bit = new uint8_t[rowNumber*columnNumber];
	bpp16tobpp8(waitStilimusdata->FrameData, arrImage8bit, rowNumber, columnNumber);

	QVector<QRgb> vcolorTable;
	for (int i = 0; i < 256; i++)
	{
		vcolorTable.append(qRgb(i, i, i));
	}
	QByteArray data;
	data.resize((rowNumber)*(columnNumber));
	for (int m = 0; m < rowNumber*columnNumber; m++)
	{
		data[m] = arrImage8bit[m];
	}
	QImage Image((const uchar*)data.constData(), columnNumber, rowNumber, columnNumber, QImage::Format_Indexed8);
	Image.setColorTable(vcolorTable);
	delete[] arrImage8bit; arrImage8bit = NULL;

	//display
	QLabel *pImageLabel = NULL;
	if (NULL != ui.TestEngineTableWidget->cellWidget(7, iPos))
		pImageLabel = static_cast<QLabel*>(ui.TestEngineTableWidget->cellWidget(7, iPos));
	else
		pImageLabel = new QLabel();

	pImageLabel->setPixmap(QPixmap::fromImage(Image));
	pImageLabel->setAlignment(Qt::AlignCenter);
	ui.TestEngineTableWidget->setCellWidget(7, iPos, pImageLabel);
	ui.TestEngineTableWidget->resizeRowToContents(7);
}

void Jingan::bpp16tobpp8(int16_t *image, uint8_t *newimage, const int num_rows, const int num_cols)
{
	int i, n;
	//unsigned char *ptr;
	int v;

	n = num_rows*num_cols;
	//ptr = (unsigned char*)image;

	for (i = 0; i < n; ++i) {
		v = ((int)image[i] + 3500) * 255 / 7000;
		if (v < 0) v = 0;
		if (v > 255) v = 255;
		newimage[i] = (unsigned char)v;
	}
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
		Run();
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