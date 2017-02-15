#pragma once
#include "Syn_BravoFingerprintTest.h"

class Ts_BravoFinalizationStep : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoFinalizationStep(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoFinalizationStep();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	FinalizationTestData *_FinalizationTestData;
};

