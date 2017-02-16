#include "Ts_BravoCalibrate.h"

Ts_BravoCalibrate::Ts_BravoCalibrate(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pCalibrateTestData(NULL)
{
}

Ts_BravoCalibrate::~Ts_BravoCalibrate()
{
}

void Ts_BravoCalibrate::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoCalibrate::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoCalibrate::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

	_pCalibrateTestData = new CalibrateTestData();
	_pCalibrateTestData->data_name = _strName;
}

void Ts_BravoCalibrate::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);
	rc = _pSynModule->FpCalibrate(CalibrateType::Firmware);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoCalibrate::FpCalibrate is failed!");
		throw Exception;
		return;
	}

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;
	uint32_t size = rowNumber*columnNumber * 2;

	uint8_t *arr_FW_BaseLine = new uint8_t[size];
	rc = _pSynModule->FpStateGet(StateGetType::FW_BL, arr_FW_BaseLine, size);
	if (0 != rc)
	{
		delete[] arr_FW_BaseLine;
		arr_FW_BaseLine = NULL;
		Exception.SetError(rc);
		Exception.SetDescription("BravoCalibrate::FpStateGet(FW_BL) is failed!");
		throw Exception;
		return;
	}
	memcpy(_pCalibrateTestData->FWBaseline, arr_FW_BaseLine, size);
	delete[] arr_FW_BaseLine;
	arr_FW_BaseLine = NULL;

	uint8_t *arr_LNA_BaseLine = new uint8_t[size];
	rc = _pSynModule->FpStateGet(StateGetType::LNA_BL, arr_LNA_BaseLine, size);
	if (0 != rc)
	{
		delete[] arr_LNA_BaseLine;
		arr_LNA_BaseLine = NULL;
		Exception.SetError(rc);
		Exception.SetDescription("BravoCalibrate::FpStateGet(LNA_BL) is failed!");
		throw Exception;
		return;
	}
	memcpy(_pCalibrateTestData->LNABaseline, arr_LNA_BaseLine, size);
	delete[] arr_LNA_BaseLine;
	arr_LNA_BaseLine = NULL;
	
	_pCalibrateTestData->executed = true;
	_pCalibrateTestData->pass = true;
}

void Ts_BravoCalibrate::ProcessData()
{
	if (_pCalibrateTestData->pass)
		_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type("Calibrate", "Pass"));
	else
		_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type("Calibrate", "Fail"));
}

void Ts_BravoCalibrate::CleanUp()
{
	CalculateTestTime(_pCalibrateTestData->test_time);

	StoreTestData(_pCalibrateTestData->data_name, static_cast<SynTestData*>(_pCalibrateTestData));
}