#include "Ts_BravoFinalizationStep.h"

Ts_BravoFinalizationStep::Ts_BravoFinalizationStep(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _FinalizationTestData(NULL)
{
}

Ts_BravoFinalizationStep::~Ts_BravoFinalizationStep()
{
}

void Ts_BravoFinalizationStep::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoFinalizationStep::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoFinalizationStep::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

	_FinalizationTestData = new FinalizationTestData();
	_FinalizationTestData->data_name = _strName;
}

void Ts_BravoFinalizationStep::Execute()
{
	uint32_t rc = 0;

	//PowerOn
	_pSynModule->PowerOff();
	
	_FinalizationTestData->executed = true;
}

void Ts_BravoFinalizationStep::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type("FinalizationStep", "Pass"));
	_FinalizationTestData->pass = true;

	if (0 == _pSynDutUtils->_pDutTestResult->list_bincodes.size())
	{
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("1");
	}
}

void Ts_BravoFinalizationStep::CleanUp()
{
	CalculateTestTime(_FinalizationTestData->test_time);

	SynTestData *pSynTestData = static_cast<SynTestData*>(_FinalizationTestData);
	_pSynDutUtils->_pDutTestResult->list_testdata.push_back(pSynTestData);

}