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

	_pSynDutUtils = new Syn_Dut_Utils();

	rc = pConfigUtilsMT->GetConfigMTInfo(_pSynDutUtils->Config_MT_Info);
	delete pConfigUtilsMT;
	pConfigUtilsMT = NULL;
	if (0 != rc)
	{
		delete _pSynDutUtils;
		_pSynDutUtils = NULL;
		return rc;
	}

	string strDutController(_pSynDutUtils->Config_MT_Info.strDutController);
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
		delete _pSynDutUtils;
		_pSynDutUtils = NULL;
		return rc;
	}

	//Module
	string strModuleType = _pSynDutUtils->Config_MT_Info.strDutType;
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
		//SensorType = FpBravoModule::Shasta;
	}
	else
	{
		SensorType = FpBravoModule::Denali;
	}
	rc = FpBravoModule::CreateModuleInstance(SensorType, _pSyn_Bridge, _pSyn_Module);
	if (0 != rc)
	{
		delete _pSynDutUtils; _pSynDutUtils = NULL;
		delete _pSyn_Bridge; _pSyn_Bridge = NULL;
		return rc;
	}

	rc = _pSyn_Bridge->SetPortSPI(clockrate);

	_State = closed;

	return rc;
}