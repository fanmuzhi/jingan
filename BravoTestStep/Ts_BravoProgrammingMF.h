#pragma once
#include "Syn_BravoFingerprintTest.h"

class Ts_BravoProgrammingMF : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoProgrammingMF(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoProgrammingMF();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	ProgrammingMFTestData *_pProgrammingMFTestData;
};