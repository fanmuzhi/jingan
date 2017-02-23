#ifndef JINGAN_H
#define JINGAN_H

#include <QtWidgets/QMainWindow>
#include "ui_jingan.h"

#include "Syn_Test_Utils.h"
#include "Syn_Dut_Utils.h"
#include "FpBravoModule.h"
#include "Syn_TestEngine.h"

//Local
#include "LocalSettings.h"
#include "LocalSettingsConfig.h"
#include "SynThread.h"
#include "SynImagingThread.h"
#include "JinganInfo.h"

//std
#include <vector>
#include <string>
using namespace std;

class Jingan : public QMainWindow
{
	Q_OBJECT

public:
	Jingan(QWidget *parent = 0);
	~Jingan();

	void ClearTestEngines();

	QString TransformEngineStatus(Syn_TestEngine::EngineState EngineState);

	void keyPressEvent(QKeyEvent * ev);

	void ManageButtonStatus(FlagType flag);

	void bpp16tobpp8(int16_t *image, uint8_t *newimage, const int num_rows, const int num_cols);
	
public Q_SLOTS:

	void Exit();

	void Initialize();

	void CreateLocalSettings();

	void DisplayInfomation();

	void Run();

	void ReceiveTestStep(unsigned int EngineNumber, const QString strTestStep, const QString strPassOrFail);

	void ReceiveTestResults(unsigned int EngineNumber, const dut_test_result *pTestData);

	void ReceivingImage(unsigned int EngineNumber, const dut_test_result *pTestData);

private:

	Ui::JinganClass ui;

	SynThread _qThreadArray[TESTENGINE_COUNTS_MAX];
	SynImagingThread _qImagingThreadArray[TESTENGINE_COUNTS_MAX];
	vector<Syn_TestEngine*> _ListOfTestEngine;

	LocalSettingConfig_t _LocalSettingConfig;

	uint32_t _finishedEngineCounts;
};

#endif // JINGAN_H
