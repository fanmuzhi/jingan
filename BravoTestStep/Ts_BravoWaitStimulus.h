#pragma once
#include "Syn_BravoFingerprintTest.h"

class Ts_BravoWaitStimulus : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoWaitStimulus(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoWaitStimulus();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	WaitStilumusTestData *_WaitStilumusTestData;
};

