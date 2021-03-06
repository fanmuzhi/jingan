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

	enum EngineState{ idle, data_ready, running, error, closed };

	enum ExcuteType{ All, Setup, Excute, ProcessData, Cleanup };

	//Syn_TestEngine();
	~Syn_TestEngine();

	//static function
	static uint32_t CreateTestEngine(uint32_t TestEngineNumber, string strDeviceSerialNumber, string strConfigFilePath, Syn_TestEngine * &opTestEngine);

	uint32_t Open();

	uint32_t GetTestData(dut_test_result * &opTestData);

	uint32_t Close();

	void GetTestStepList(vector<string> &oListTeststepName);

	uint32_t ExecuteTestStep(string TestStepName, ExcuteType Type = All);

	uint32_t WriteLog(string strFolderPath, string strFileName = "");

	inline void GetRowColumn(uint32_t &rowNumber, uint32_t &columnNumber)
	{
		rowNumber = _Config_MT_Info.rowNumber;
		columnNumber = _Config_MT_Info.columnNumber;
	}

	inline EngineState GetStatus(){ return _State; }

	inline void GetTestEngineNumber(unsigned int &oEngineNumber){ oEngineNumber = _TestEngineNumber; };

	inline void GetDeviceSerialNumber(string &oSerialNumber){ oSerialNumber = _strDeviceSerialNumber; };

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

	Syn_Config_MT_Info _Config_MT_Info;

	EngineState	_State;
};

