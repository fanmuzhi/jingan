#pragma once
#include "Syn_BravoFingerprintTest.h"

class Ts_BravoSNRTest : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoSNRTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);

	virtual ~Ts_BravoSNRTest();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	SNRTestData *_pSNRTestData;
};

