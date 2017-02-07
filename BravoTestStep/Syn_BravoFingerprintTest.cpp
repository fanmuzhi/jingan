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
		return 1;
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