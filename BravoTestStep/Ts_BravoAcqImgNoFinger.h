#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoAcqImgNoFinger :	public Syn_BravoFingerprintTest
{
public:
	Ts_BravoAcqImgNoFinger(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoAcqImgNoFinger();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

protected:

	AcqImageNoFingerTestData *_pAcqImageNoFingerTestData;
};

