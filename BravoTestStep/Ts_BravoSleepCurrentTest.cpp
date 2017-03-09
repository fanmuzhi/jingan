#include "Ts_BravoSleepCurrentTest.h"

Ts_BravoSleepCurrentTest::Ts_BravoSleepCurrentTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pSleepCurrentTestData(NULL)
{
}

Ts_BravoSleepCurrentTest::~Ts_BravoSleepCurrentTest()
{
}

void Ts_BravoSleepCurrentTest::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoSleepCurrentTest::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoSleepCurrentTest::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pSleepCurrentTestData = new SleepCurrentTestData();
	_pSleepCurrentTestData->data_name = _strName;
	_pSleepCurrentTestData->highLimit = 1 * 1000;	//nA
	_pSleepCurrentTestData->lowLimit = 0;			//nA
	_pSleepCurrentTestData->delay = 500;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);
	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();
	if (iListSize < 3)
	{
		for (size_t t = 1; t <= 3 - iListSize; t++)
			listOfArgValue.push_back(std::string(""));
	}
	if (0 != listOfArgValue[0].length())
		_pSleepCurrentTestData->highLimit = std::stof(listOfArgValue[0]);
	if (0 != listOfArgValue[1].length())
		_pSleepCurrentTestData->lowLimit = std::stof(listOfArgValue[1]);
	if (0 != listOfArgValue[2].length())
		_pSleepCurrentTestData->delay = atoi(listOfArgValue[2].c_str());
}

void Ts_BravoSleepCurrentTest::Execute()
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
		Exception.SetDescription("BravoSleepCurrentTest::PowerOn() failed");
		throw Exception;
	}

	rc = _pSynModule->FpTidleSet(500);

	::Sleep(_pSleepCurrentTestData->delay);

	uint32_t arrValue[2] = {0};
	rc = _pSynBridge->GetCurrentValues(arrValue, true);		//low gain
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoSleepCurrentTest::GetCurrentValues() failed");
		throw Exception;
	}

	float spivcc_current = ((float)(arrValue[0])) / 1000;
	float vcc_current = ((float)(arrValue[1])) / 1000;

	_pSleepCurrentTestData->spivcc_current_uA = spivcc_current;	//uA
	_pSleepCurrentTestData->vcc_current_uA = vcc_current;		//uA

	_pSleepCurrentTestData->executed = true;

	rc = _pSynModule->FpTidleSet(0);
}

void Ts_BravoSleepCurrentTest::ProcessData()
{
	_pSleepCurrentTestData->pass = true;

	if (0 != _pSleepCurrentTestData->spivcc_current_uA)
	{
		if (_pSleepCurrentTestData->spivcc_current_uA > _pSleepCurrentTestData->highLimit
			|| _pSleepCurrentTestData->vcc_current_uA > _pSleepCurrentTestData->highLimit
			|| _pSleepCurrentTestData->vcc_current_uA < _pSleepCurrentTestData->lowLimit
			|| _pSleepCurrentTestData->spivcc_current_uA < _pSleepCurrentTestData->lowLimit)
		{
			_pSleepCurrentTestData->pass = false;
			_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("117");
		}
	}
	else
	{
		if (_pSleepCurrentTestData->vcc_current_uA > _pSleepCurrentTestData->highLimit
			|| _pSleepCurrentTestData->vcc_current_uA < _pSleepCurrentTestData->lowLimit)
		{
			_pSleepCurrentTestData->pass = false;
			_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("117");
		}
	}

	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pSleepCurrentTestData->data_name, _pSleepCurrentTestData->pass ? "Pass" : "Fail"));
}

void Ts_BravoSleepCurrentTest::CleanUp()
{
	CalculateTestTime(_pSleepCurrentTestData->test_time);

	StoreTestData(_pSleepCurrentTestData->data_name, static_cast<SynTestData*>(_pSleepCurrentTestData));
}