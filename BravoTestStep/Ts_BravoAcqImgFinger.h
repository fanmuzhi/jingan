#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoAcqImgFinger : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoAcqImgFinger(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoAcqImgFinger();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	AcqImageFingerTestData *_pAcqImageFingerTestData;

};

