#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoSleepCurrentTest : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoSleepCurrentTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoSleepCurrentTest();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	SleepCurrentTestData *_pSleepCurrentTestData;
};

