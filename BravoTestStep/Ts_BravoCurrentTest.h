#pragma once


#include "Syn_BravoFingerprintTest.h"
class Ts_BravoCurrentTest : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoCurrentTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoCurrentTest();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	CurrentTestData *_pCurrentTestData;
};

