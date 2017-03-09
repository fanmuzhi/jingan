#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoUpdateFIB : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoUpdateFIB(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoUpdateFIB();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	UpdateFIBTestData *_pUpdateFIBTestData;
};

