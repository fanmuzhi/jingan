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
	if (NULL == _pSynModule)
	{
		return;
	}
	if (NULL == _pSynDutUtils)
	{
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
	_FinalizationTestData->pass = true;
}

void Ts_BravoFinalizationStep::CleanUp()
{
	CalculateTestTime(_FinalizationTestData->test_time);

	SynTestData *pSynTestData = static_cast<SynTestData*>(_FinalizationTestData);
	_pSynDutUtils->_pDutTestResult->list_testdata.push_back(pSynTestData);
}