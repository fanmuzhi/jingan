#pragma once
#include "Syn_TestStep.h"
#include "Syn_Exception.h"

#include <windows.h>
#include <time.h>

class Syn_BravoFingerprintTest : public Syn_TestStep
{
public:

	Syn_BravoFingerprintTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Syn_BravoFingerprintTest();

	void CalculateTestTime(double &ioTestTime);

	uint32_t ParseTestStepArgs(const string &strArgsValue, vector<string> &oListOfArgValue, string strSymbol = " ");

protected:

	ULONGLONG _start_time, _finish_time;
};

