#pragma once

//Local
#include "sjimportexport.h"
#include "Synaptics_Utils.h"

//std
#include <string>
#include <vector>

class Syn_Site;

class SYNA_SJ_API Synaptics_Site
{

public:
	
	Synaptics_Site(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath, const SynapticsAdcBaseLineInfo &adcInfo);
	~Synaptics_Site();

	uint32_t Open();
	
	std::vector<std::string> GetTestStepList();

	uint32_t ExecuteTestStep(std::string strTestStepName);

	uint32_t GetTestResult(Synaptics_TestResult * &opSynapticsTestResult);
	
	void WriteLog(std::string strFolderPath, std::string strFileName);

	uint32_t Close();

private:

	Syn_Site *_pSyn_Site;

	Synaptics_TestResult *_pSynaptics_TestResult;
};

