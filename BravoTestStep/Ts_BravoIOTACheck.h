#pragma once

#include "Syn_BravoFingerprintTest.h"

class Ts_BravoIOTACheck : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoIOTACheck(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoIOTACheck();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	void IotaParse(vcsfw_reply_iota_find_hdr_t iotafindheader, uint8_t *arrIota, uint32_t startPos, uint32_t endPos, string &oIOTAType, string &oIOTAValue);

	void IotaFrameParse(uint8_t *arrIotaFrame, uint32_t startPos, uint32_t endPos, string &oFrameTag, string &oFrameData);

	string ConvertData(uint8_t *arrData, uint32_t startPos, uint32_t endPos);

	uint32_t GetBitsValue(uint32_t value, uint8_t startIndex, uint32_t length);

protected:

	IOTACheckTestData *_pIOTACheckTestData;
};

