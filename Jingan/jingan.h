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


public Q_SLOTS:

	void Exit();

	void Initialize();

	void CreateLocalSettings();


private:

	Ui::JinganClass ui;

	vector<Syn_TestEngine*> _ListOfTestEngine;

	LocalSettingConfig_t _LocalSettingConfig;
};

#endif // JINGAN_H
