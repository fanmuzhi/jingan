#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoImperfections : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoImperfections(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoImperfections();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	ImperfectionsTestData *_pImperfectionsTestData;
};