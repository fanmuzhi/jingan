#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoHuaweiImageTest : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoHuaweiImageTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	
	virtual ~Ts_BravoHuaweiImageTest();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	HuaweiImageTestData *_pHuaweiImageTestData;
};

