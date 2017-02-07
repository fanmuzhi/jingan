#include "Syn_TestEngine.h"

Syn_TestEngine::Syn_TestEngine(uint32_t EngineNumber, string strDeviceSerNumber, string strConfigFilePath)
:_TestEngineNumber(EngineNumber)
, _strDeviceSerialNumber(strDeviceSerNumber)
, _strConfigFilePath(strConfigFilePath)
, _pSynDutUtils(NULL)
, _pSyn_Bridge(NULL)
, _pSyn_Module(NULL)
, _State(closed)
{
}

Syn_TestEngine::~Syn_TestEngine()
{
	if (NULL != _pSynDutUtils)
	{
		delete _pSynDutUtils;
		_pSynDutUtils = NULL;
	}

	if (NULL != _pSyn_Bridge)
	{
		delete _pSyn_Bridge;
		_pSyn_Bridge = NULL;
	}

	if (NULL != _pSyn_Module)
	{
		delete _pSyn_Module;
		_pSyn_Module = NULL;
	}
}

uint32_t Syn_TestEngine::CreateTestEngine(uint32_t TestEngineNumber, string strDeviceSerialNumber, string strConfigFilePath, Syn_TestEngine * &opTestEngine)
{
	opTestEngine = NULL;

	uint32_t rc = 0;

	opTestEngine = new Syn_TestEngine(TestEngineNumber, strDeviceSerialNumber, strConfigFilePath);
	rc = opTestEngine->Init();
	if (0 != rc)
	{
		delete opTestEngine;
		opTestEngine = NULL;
	}

	return rc;

}

uint32_t Syn_TestEngine::Init()
{
	_State = error;

	//xml config file parse
	Syn_Config_Utils_MT *pConfigUtilsMT = NULL;
	uint32_t rc = Syn_Config_Utils_MT::CreateConfigUtils_MT(_strConfigFilePath, pConfigUtilsMT);
	if (0!=rc || NULL == pConfigUtilsMT)
	{
		return rc;
	}

	rc = pConfigUtilsMT->GetConfigMTInfo(_Config_MT_Info);
	delete pConfigUtilsMT;
	pConfigUtilsMT = NULL;
	if (0 != rc)
	{
		return rc;
	}

	string strDutController(_Config_MT_Info.strDutController);
	devicetype DeviceType;
	uint32_t clockrate = M5_CLOCKRATE;
	if (std::string("MPC04") == strDutController)
	{
		DeviceType = spi_mpc04;
		clockrate = MPC04_CLOCKRATE;
	}
	else
	{
		DeviceType = spi_m5;
		clockrate = M5_CLOCKRATE;
	}

	//Create Bridge
	rc = syn_bridge::CreateDeviceInstance(_strDeviceSerialNumber, DeviceType, _pSyn_Bridge);
	if (0 != rc)
	{
		return rc;
	}

	//Module
	string strModuleType = _Config_MT_Info.strDutType;
	FpBravoModule::BravoSensorType SensorType;
	if (string("Denali") == strModuleType)
	{
		SensorType = FpBravoModule::Denali;
	}
	else if (std::string("Hayes") == strModuleType)
	{
		SensorType = FpBravoModule::Hayes;
	}
	else if (std::string("Shasta") == strModuleType)
	{
		SensorType = FpBravoModule::Shasta;
	}
	else
	{
		SensorType = FpBravoModule::Denali;
	}
	rc = FpBravoModule::CreateModuleInstance(SensorType, _pSyn_Bridge, _pSyn_Module);
	if (0 != rc)
	{
		delete _pSyn_Bridge; 
		_pSyn_Bridge = NULL;
		return rc;
	}

	rc = _pSyn_Bridge->SetPortSPI(clockrate);

	_State = closed;

	return rc;
}

uint32_t Syn_TestEngine::Open()
{
	uint32_t rc = 0;

	if (_State == running)
	{
		return ERROR_ENGINE_STATE;
	}

	//Create DutUtils
	if (NULL != _pSynDutUtils)
	{
		delete _pSynDutUtils;
		_pSynDutUtils = NULL;
	}
	_pSynDutUtils = new Syn_Dut_Utils();
	_pSynDutUtils->Config_MT_Info = _Config_MT_Info;

	//fill info

	size_t stringSize = _strConfigFilePath.size();
	size_t lastBackslashIndex = _strConfigFilePath.find_last_of("/");
	if (std::string::npos == lastBackslashIndex)
	{
		lastBackslashIndex = _strConfigFilePath.find_last_of("\\");
	}
	//_pSyn_Dut->_sConfigFileName = _strConfigFilePath.substr(lastBackslashIndex + 1, stringSize - lastBackslashIndex - 1);

	_State = idle;

	//_pSyn_Dut->_DeviceSerialNumber = _strSerialNumber;
	//_pSyn_Dut->_iSiteNumber = _iSiteNumber;

	return 0;
}

uint32_t Syn_TestEngine::GetTestData(dut_test_result * &opTestData)
{
	opTestData = NULL;

	if (_State == error || _State != data_ready)
	{
		return ERROR_ENGINE_STATE;
	}

	if (NULL != _pSynDutUtils)
	{
		opTestData = _pSynDutUtils->_pDutTestResult;

		_State = idle;
		return 0;
	}

	_State = error;

	return ERROR_ENGINE_DATA;
}

uint32_t Syn_TestEngine::Close()
{
	if (_State == running)
	{
		return ERROR_ENGINE_STATE;
	}

	if (NULL != _pSynDutUtils)
	{
		delete _pSynDutUtils;
		_pSynDutUtils = NULL;
	}

	_State = closed;

	return 0;
}

void Syn_TestEngine::GetTestStepList(std::vector<std::string> &oListTeststepName)
{
	oListTeststepName.clear();

	for (size_t i = 1; i <= _Config_MT_Info.list_TestStep_Info.size(); i++)
	{
		oListTeststepName.push_back(_Config_MT_Info.list_TestStep_Info[i - 1].strTestStepName);
	}
}

uint32_t Syn_TestEngine::ExecuteTestStep(string TestStepName, ExcuteType Type)
{
	uint32_t rc = 0;

	if (_State == error)
	{
		ERROR_ENGINE_STATE;
	}
	if (_State != idle && _State != data_ready)
	{
		return ERROR_ENGINE_STATE;
	}

	string strArgsValue("");
	_Config_MT_Info.GetTestStepInfo(TestStepName, strArgsValue);
	Syn_TestStep *pTestStep = NULL;
	rc = Syn_TestStepFactory::CreateBravoTestStep(TestStepName, strArgsValue, _pSyn_Module, _pSynDutUtils, pTestStep);
	if (0 != rc || NULL == pTestStep)
	{
		_State = error;
		return ERROR_TESTSTEP;
	}

	_State = running;
	try
	{
		switch (Type)
		{
			case All:
				pTestStep->SetUp();
				pTestStep->Execute();
				pTestStep->ProcessData();
				pTestStep->CleanUp();
				break;

			case Setup:
				pTestStep->SetUp();
				break;

			case Excute:
				pTestStep->Execute();
				break;

			case ProcessData:
				pTestStep->ProcessData();
				break;

			case Cleanup:
				pTestStep->CleanUp();
				break;

			default:
				pTestStep->SetUp();
				pTestStep->Execute();
				pTestStep->ProcessData();
				pTestStep->CleanUp();
				break;
		}

		delete pTestStep;
		pTestStep = NULL;

		_State = data_ready;
	}
	catch (...)
	{
		try
		{
			_pSyn_Bridge->SetVoltages(0, 0);
		}
		catch (...){}
		delete pTestStep;
		pTestStep = NULL;

		_State = error;
		return ERROR_TESTSTEP;
	}

	return 0;
}