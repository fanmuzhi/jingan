#include "Ts_BravoDRdyTest.h"

Ts_BravoDRdyTest::Ts_BravoDRdyTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pDrdyTestData(NULL)
{
}

Ts_BravoDRdyTest::~Ts_BravoDRdyTest()
{
}

void Ts_BravoDRdyTest::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoDRdyTest::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoDRdyTest::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

	_pDrdyTestData = new DrdyTestData();
	_pDrdyTestData->data_name = _strName;
}

void Ts_BravoDRdyTest::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	uint8_t  pResult[2] = { 0, 0 };
	bool nDRdyStates = false;

	rc = _pSynModule->PowerOff();
	rc = _pSynModule->PowerOn(_pSynDutUtils->Config_MT_Info.vddh_mv, _pSynDutUtils->Config_MT_Info.vdd_mv);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoDRdyTest::PowerOn() failed");
		throw Exception;
	}

	int timeoutVal = 2000;
	do{
		_pSynBridge->GPIO_CheckDRDY(&nDRdyStates);
		if (nDRdyStates == true)
		{
			break;
		}
		timeoutVal--;
	} while (0 != timeoutVal);

	for (int i = 0; i < 2; i++)
	{
		_pSynBridge->GPIO_CheckDRDY(&nDRdyStates);
		_pDrdyTestData->arrHiStates[i] = nDRdyStates;
	}

	//excute command can set drdy 0
	rc = _pSynModule->FpTidleSet(0);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoDRdyTest::FpTidleSet() failed");
		throw Exception;
	}
	for (int i = 0; i < 2; i++)
	{
		_pSynBridge->GPIO_CheckDRDY(&nDRdyStates);
		_pDrdyTestData->arrLoStates[i] = nDRdyStates;
	}

	_pDrdyTestData->executed = true;
}

void Ts_BravoDRdyTest::ProcessData()
{
	_pDrdyTestData->pass = true;
	for (int i = 0; i < 2; i++)
	{
		if (_pDrdyTestData->arrHiStates[i] == 0)
			_pDrdyTestData->pass = false;
		if (_pDrdyTestData->arrLoStates[i] == 1)
			_pDrdyTestData->pass = false;
	}

	if (!_pDrdyTestData->pass)
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("104");

	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pDrdyTestData->data_name, _pDrdyTestData->pass ? "Pass" : "Fail"));
}

void Ts_BravoDRdyTest::CleanUp()
{
	CalculateTestTime(_pDrdyTestData->test_time);

	StoreTestData(_pDrdyTestData->data_name, static_cast<SynTestData*>(_pDrdyTestData));
}