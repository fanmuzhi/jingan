#pragma once

#include "syn_bridge.h"
#include "FpBravoModule.h"

//DutUtls
#include "Syn_Test_Utils.h"
#include "Syn_Dut_Utils.h"
#include "Syn_Config_Utils_MT.h"

//TestStep
#include "Syn_TestStep.h"
#include "Syn_TestStepFactory.h"

class Syn_TestEngine
{
public:

	enum EngineState{ idle = 0x8001, data_ready, running, error, closed };

	//Syn_TestEngine();
	~Syn_TestEngine();

	//static function
	static uint32_t CreateTestEngine(uint32_t TestEngineNumber, string strDeviceSerialNumber, string strConfigFilePath, Syn_TestEngine * &opTestEngine);

	uint32_t Open();

private:

	Syn_TestEngine(uint32_t EngineNumber, string strDeviceSerNumber, string strConfigFilePath);

	uint32_t Init();

private:

	uint32_t _TestEngineNumber;
	string _strDeviceSerialNumber;

	string _strConfigFilePath;

	Syn_Dut_Utils *_pSynDutUtils;

	syn_bridge	  *_pSyn_Bridge;
	FpBravoModule *_pSyn_Module;

	EngineState	_State;
};

