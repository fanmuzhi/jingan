#pragma once

#include "Syn_BravoFingerprintTest.h"

typedef struct half_word_frame_data_s
{
	uint32_t   frame_cols;
	uint32_t   frame_rows;
	int16_t    data[BRAVO_IMAGE_RAWDATA_MAX];    /* This is the data that is output to support the get frame request.*/
} half_word_frame_data_t;

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