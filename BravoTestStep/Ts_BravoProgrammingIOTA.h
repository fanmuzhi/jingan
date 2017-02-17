#pragma once
#include "Syn_BravoFingerprintTest.h"
class Ts_BravoProgrammingIOTA : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoProgrammingIOTA(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoProgrammingIOTA();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	ProgrammingIOTATestData *_pProgrammingIOTATestData;
};

