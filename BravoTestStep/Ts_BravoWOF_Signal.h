#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoWOF_Signal : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoWOF_Signal(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoWOF_Signal();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	WOF_SignalTestData *_pWOF_SignalTestData;
};

