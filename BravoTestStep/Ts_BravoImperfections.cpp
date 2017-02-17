#include "Ts_BravoImperfections.h"

Ts_BravoImperfections::Ts_BravoImperfections(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
: Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pImperfectionsTestData(NULL)
{
}

Ts_BravoImperfections::~Ts_BravoImperfections()
{
}

void Ts_BravoImperfections::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoImperfections::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoImperfections::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pImperfectionsTestData = new ImperfectionsTestData();
	_pImperfectionsTestData->data_name = _strName;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);
	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

	if (iListSize < 3)
	{
		for (size_t t = 1; t <= 3 - iListSize; t++)
			listOfArgValue.push_back("");
	}

	if (0 != listOfArgValue[0].length())
		_pImperfectionsTestData->m_peggedThreshold = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pImperfectionsTestData->m_flooredThreshold = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pImperfectionsTestData->m_maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
}

void Ts_BravoImperfections::Execute()
{
	_pImperfectionsTestData->executed = true;
	//Image

	_pImperfectionsTestData->pass = true;
}

void Ts_BravoImperfections::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pImperfectionsTestData->data_name, _pImperfectionsTestData->pass ? "Pass" : "Fail"));
	if (!(_pImperfectionsTestData->pass))
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("129");
}

void Ts_BravoImperfections::CleanUp()
{
	CalculateTestTime(_pImperfectionsTestData->test_time);
	
	StoreTestData(_pImperfectionsTestData->data_name, static_cast<SynTestData*>(_pImperfectionsTestData));
}