#pragma once
#include "Syn_BravoFingerprintTest.h"

class Ts_BravoCalibrate : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoCalibrate(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoCalibrate();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	CalibrateTestData *_pCalibrateTestData;
};

