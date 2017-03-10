#include "Ts_BravoInitializationStep.h"

Ts_BravoInitializationStep::Ts_BravoInitializationStep(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pInitTestData(NULL)
{
}

Ts_BravoInitializationStep::~Ts_BravoInitializationStep()
{
}

void Ts_BravoInitializationStep::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoInitializationStep::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoInitializationStep::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	//vector<string> listOfArgValue;
	//ParseTestStepArgs(strTestArgs, listOfArgValue);
	//size_t iListSize = listOfArgValue.size();
	
	_pInitTestData = new InitializationTestData();
	_pInitTestData->data_name = _strName;
}

void Ts_BravoInitializationStep::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	//PowerOn
	_pSynModule->PowerOff();
	rc = _pSynModule->PowerOn(_pSynDutUtils->Config_MT_Info.vddh_mv, _pSynDutUtils->Config_MT_Info.vdd_mv);
	if (0 != rc)
	{
		Exception.SetError(rc);
		throw Exception;
		return;
	}
	/*rc = _pSynModule->FpTidleSet(0);
	if (0 != rc)
	{
		Exception.SetError(rc);
		throw Exception;
		return;
	}*/

	vcsfw_reply_get_version_t get_version;
	rc = _pSynModule->FpGetVersion((uint8_t*)&get_version, sizeof(vcsfw_reply_get_version_t), 100);
	if (0 != rc)
	{
		Exception.SetError(rc);
		throw Exception;
		return;
	}

	//copy serial number to string
	char SingleNumber[3] = { 0 };
	sprintf(SingleNumber, "%02X", (get_version.serial_number)[0]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[0]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[1]);

	sprintf(SingleNumber, "%02X", (get_version.serial_number)[1]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[0]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[1]);
	
	sprintf(SingleNumber, "%02X", (get_version.serial_number)[2]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[0]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[1]);

	sprintf(SingleNumber, "%02X", (get_version.serial_number)[3]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[0]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[1]);

	sprintf(SingleNumber, "%02X", (get_version.serial_number)[4]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[0]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[1]);

	sprintf(SingleNumber, "%02X", (get_version.serial_number)[5]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[0]);
	_pInitTestData->strSensorSerialNumber.push_back(SingleNumber[1]);

	_pInitTestData->executed = true;
}

void Ts_BravoInitializationStep::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type("InitializationStep", "Pass"));
	_pInitTestData->pass = true;
	_pSynDutUtils->_pDutTestResult->strSensorSerialNumber = _pInitTestData->strSensorSerialNumber;
}

void Ts_BravoInitializationStep::CleanUp()
{
	CalculateTestTime(_pInitTestData->test_time);

	//SynTestData *pSynTestData = static_cast<SynTestData*>(_pInitTestData);
	//_pSynDutUtils->_pDutTestResult->list_testdata.push_back(pSynTestData);
	StoreTestData(_pInitTestData->data_name, static_cast<SynTestData*>(_pInitTestData));
}