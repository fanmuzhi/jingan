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
	
public Q_SLOTS:

	void Exit();

	void Initialize();

	void CreateLocalSettings();

	void Run();

	void ReceiveTestStep(uint32_t EngineNumber, const QString strTestStep, const QString strPassOrFail);

private:

	Ui::JinganClass ui;

	SynThread _qThreadArray[TESTENGINE_COUNTS_MAX];
	vector<Syn_TestEngine*> _ListOfTestEngine;

	LocalSettingConfig_t _LocalSettingConfig;

	uint32_t _finishedEngineCounts;
};

#endif // JINGAN_H
