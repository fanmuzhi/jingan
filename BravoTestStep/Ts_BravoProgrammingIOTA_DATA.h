#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoProgrammingIOTA_DATA : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoProgrammingIOTA_DATA(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoProgrammingIOTA_DATA();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	ProgrammingIOTA_Data_TestData *_pProgrammingIOTA_Data_TestData;
};

