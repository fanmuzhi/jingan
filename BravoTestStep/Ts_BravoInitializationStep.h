#pragma once
#include "Syn_BravoFingerprintTest.h"

class Ts_BravoInitializationStep : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoInitializationStep(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoInitializationStep();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	InitializationTestData *_pInitTestData;
};

