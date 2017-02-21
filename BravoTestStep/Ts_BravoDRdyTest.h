#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoDRdyTest : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoDRdyTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoDRdyTest();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	DrdyTestData *_pDrdyTestData;
};

