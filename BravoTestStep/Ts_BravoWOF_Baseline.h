#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoWOF_Baseline : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoWOF_Baseline(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoWOF_Baseline();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	WOF_BaselineTestData *_pWOF_BaselineTestData;
};

