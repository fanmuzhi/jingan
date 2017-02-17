#include "Syn_BravoFingerprintTest.h"

Syn_BravoFingerprintTest::Syn_BravoFingerprintTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_TestStep(strName, pSynModule, pSynDutUtils)
, _start_time(0)
, _finish_time(0)
{
	_start_time = GetTickCount64();
}

Syn_BravoFingerprintTest::~Syn_BravoFingerprintTest()
{
}

void Syn_BravoFingerprintTest::CalculateTestTime(double &ioTestTime)
{
	_finish_time = GetTickCount64();
	ioTestTime = (double)(_finish_time - _start_time);
}

uint32_t Syn_BravoFingerprintTest::ParseTestStepArgs(const string &strArgsValue, vector<string> &oListOfArgValue, string strSymbol)
{
	oListOfArgValue.clear();

	unsigned int iLength = strArgsValue.length();
	if (0 == iLength)
	{
		return ERROR_TSETSTEP_ARGSLENGTH;
	}

	string strTempValue = strArgsValue;
	while (true)
	{
		size_t SymbolPosition = strTempValue.find(strSymbol);
		if (SymbolPosition == std::string::npos)
		{
			if (0 != strTempValue.length())
			{
				oListOfArgValue.push_back(strTempValue);
			}
			break;
		}

		string strArgValue = strTempValue.substr(0, SymbolPosition);
		oListOfArgValue.push_back(strArgValue);
		strTempValue = strTempValue.substr(SymbolPosition + 1, strTempValue.length() - SymbolPosition);
	}

	return 0;
}

SynTestData * Syn_BravoFingerprintTest::RetrieveTestData(string strTestStepName)
{
	SynTestData *pTestData = NULL;

	if (NULL == _pSynDutUtils)
		return NULL;
	if (NULL == _pSynDutUtils->_pDutTestResult)
		return NULL;

	for (size_t t = 1; t <= _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
	{
		SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t - 1];
		if (NULL != Test_data)
		{
			if (strTestStepName == Test_data->data_name)
			{
				pTestData = _pSynDutUtils->_pDutTestResult->list_testdata[t - 1];
				break;
			}
		}
	}

	return pTestData;
}

void Syn_BravoFingerprintTest::StoreTestData(string strTestStepName, SynTestData *pTestData)
{
	if (NULL == pTestData)
		return;
	if (NULL == _pSynDutUtils)
		return;
	if (NULL == _pSynDutUtils->_pDutTestResult)
		return;

	bool IsExists(false);
	for (size_t t = 1; t <= _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
	{

		SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t - 1];
		if (NULL != Test_data)
		{
			if (strTestStepName == Test_data->data_name)
			{
				_pSynDutUtils->_pDutTestResult->list_testdata[t - 1] = pTestData;
				IsExists = true;
				break;
			}
		}
	}

	if (!IsExists)
		_pSynDutUtils->_pDutTestResult->list_testdata.push_back(pTestData);
}

void Syn_BravoFingerprintTest::bpp16tobpp8(int16_t *image, uint8_t *newimage, const int num_rows, const int num_cols)
{
	int i, n;
	//unsigned char *ptr;
	int v;

	n = num_rows*num_cols;
	//ptr = (unsigned char*)image;

	for (i = 0; i < n; ++i) {
		v = ((int)image[i] + 3500) * 255 / 7000;
		if (v < 0) v = 0;
		if (v > 255) v = 255;
		newimage[i] = (unsigned char)v;
	}
}