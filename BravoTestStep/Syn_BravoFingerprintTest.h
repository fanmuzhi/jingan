#pragma once
#include "Syn_TestStep.h"
#include "Syn_Exception.h"

#include <windows.h>
#include <time.h>

#include "vcsfw\vcsTypes.h"
#include "vcsfw\vcsfw_v4.h"

class Syn_BravoFingerprintTest : public Syn_TestStep
{
public:

	Syn_BravoFingerprintTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Syn_BravoFingerprintTest();

	void CalculateTestTime(double &ioTestTime);

	uint32_t ParseTestStepArgs(const string &strArgsValue, vector<string> &oListOfArgValue, string strSymbol = " ");

	SynTestData * RetrieveTestData(string strTestStepName);

	void StoreTestData(string strTestStepName, SynTestData *pTestData);

	void bpp16tobpp8(int16_t *image, uint8_t *newimage, const int num_rows, const int num_cols);

protected:

	ULONGLONG _start_time, _finish_time;
};

