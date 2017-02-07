#ifndef JINGAN_H
#define JINGAN_H

#include <QtWidgets/QMainWindow>
#include "ui_jingan.h"

#include "Syn_Test_Utils.h"
#include "Syn_Dut_Utils.h"
#include "FpBravoModule.h"
#include "Syn_TestEngine.h"

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

public Q_SLOTS:

	void Exit();

private:

	Ui::JinganClass ui;

	vector<Syn_TestEngine*> _ListOfTestEngine;
};

#endif // JINGAN_H
