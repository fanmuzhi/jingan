#pragma once
#include "Syn_BravoFingerprintTest.h"


class Ts_BravoSharpness : public Syn_BravoFingerprintTest
{
public:
	Ts_BravoSharpness(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	
	virtual ~Ts_BravoSharpness();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	SharpnessData *_pSharpnessData;

};

