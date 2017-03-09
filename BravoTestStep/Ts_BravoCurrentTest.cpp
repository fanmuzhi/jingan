#include "Ts_BravoCurrentTest.h"

Ts_BravoCurrentTest::Ts_BravoCurrentTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pCurrentTestData(NULL)
{
}

Ts_BravoCurrentTest::~Ts_BravoCurrentTest()
{
}

void Ts_BravoCurrentTest::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoCurrentTest::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoCurrentTest::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

	_pCurrentTestData = new CurrentTestData();
	_pCurrentTestData->data_name = _strName;
	_pCurrentTestData->m_nImageAcqVccMax_uA = (float)(50 * 1000);
	_pCurrentTestData->m_nImageAcqSpivccMax_uA = (float)(5 * 1000);
	_pCurrentTestData->m_nImageAcqVccMin_uA = (float)(1 * 1000);
	_pCurrentTestData->m_nImageAcqSpivccMin_uA = (float)(0.01 * 1000);

	if (iListSize < 4)
	{
		for (size_t t = 1; t <= 4 - iListSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pCurrentTestData->m_nImageAcqVccMax_uA = (float)(stof(listOfArgValue[0]));
	if (0 != listOfArgValue[1].length())
		_pCurrentTestData->m_nImageAcqSpivccMax_uA = (float)(stof(listOfArgValue[1]));
	if (0 != listOfArgValue[2].length())
		_pCurrentTestData->m_nImageAcqVccMin_uA = (float)(stof(listOfArgValue[2]));
	if (0 != listOfArgValue[3].length())
		_pCurrentTestData->m_nImageAcqSpivccMin_uA = (float)(stof(listOfArgValue[3]));
}

void Ts_BravoCurrentTest::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;

	//read current
	//since it's high range, the spi_vcc current is not accurate.
	uint32_t arrValue[2] = { 0, 0 };
	uint8_t *arr_8bitsFrame = new uint8_t[rowNumber * columnNumber * 2];
	rc = _pSynModule->FpGetImage(arr_8bitsFrame, rowNumber * columnNumber * 2, arrValue);
	delete[] arr_8bitsFrame;
	arr_8bitsFrame = NULL;
	_pSynModule->FpFrameFinish();
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoCurrentTest::FpGetImage Failed");
		throw Exception;
	}

	_pCurrentTestData->ImageAcqSpivccCurrent_uA = ((float)arrValue[0]) / 1000;	//hign gain 1.8V
	_pCurrentTestData->ImageAcqVccCurrent_uA = ((float)arrValue[1]) / 1000;	//hign gain 3.3V

	_pCurrentTestData->executed = true;
}

void Ts_BravoCurrentTest::ProcessData()
{
	_pCurrentTestData->pass = true;
	
	if ((_pCurrentTestData->ImageAcqSpivccCurrent_uA > _pCurrentTestData->m_nImageAcqSpivccMax_uA) ||
		(_pCurrentTestData->ImageAcqSpivccCurrent_uA < _pCurrentTestData->m_nImageAcqSpivccMin_uA))
		_pCurrentTestData->pass = false;

	if (_pCurrentTestData->ImageAcqVccCurrent_uA > _pCurrentTestData->m_nImageAcqVccMax_uA)
		_pCurrentTestData->pass = 0;

	if (!_pCurrentTestData->pass)
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("115");

	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pCurrentTestData->data_name, _pCurrentTestData->pass ? "Pass" : "Fail"));
}

void Ts_BravoCurrentTest::CleanUp()
{
	CalculateTestTime(_pCurrentTestData->test_time);

	StoreTestData(_pCurrentTestData->data_name, static_cast<SynTestData*>(_pCurrentTestData));
}