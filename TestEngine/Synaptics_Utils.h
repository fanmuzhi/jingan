#pragma once

//local
#include "sjimportexport.h"

#include <stdint.h>

//std
#include <vector>
#include <map>
#include <string>

struct Synaptics_TestResult
{
	std::string strSensorNumber;

	std::map<std::string, std::string> mapStepResult;

	float snrValue;
	float huaweiSnrValue;

	std::vector<std::string> listOfBincodes;

	uint8_t arrImageNoFinger[200][200];
	uint8_t ImageNoFingerRow;
	uint8_t ImageNoFingerCol;

	uint8_t arrImageFinger[200][200];
	uint8_t ImageFingerRow;
	uint8_t ImageFingerCol;
};

struct SynapticsAdcBaseLineInfo
{
	uint32_t m_nVdd;
	uint32_t m_nVio;
	uint32_t m_nVled;
	uint32_t m_nVddh;

	uint32_t arrAdcBaseLines[4][4];
};