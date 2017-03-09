#include "Ts_BravoSNRTest.h"
#include "synImageTest.h"

typedef float(_stdcall *snrTest)(unsigned char*, unsigned char*, int, int, int, int *, float *, float fcrop);

Ts_BravoSNRTest::Ts_BravoSNRTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pSNRTestData(NULL)
{
}

Ts_BravoSNRTest::~Ts_BravoSNRTest()
{
}

void Ts_BravoSNRTest::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoSNRTest::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoSNRTest::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pSNRTestData = new SNRTestData();
	_pSNRTestData->data_name = _strName;
	
	_pSNRTestData->signal_overall_high = 4000;
	_pSNRTestData->signal_overall_low = 800;
	_pSNRTestData->noise_overall_high = 10;
	_pSNRTestData->noise_overall_low = 0;
	_pSNRTestData->snr_overall_high = 1200;
	_pSNRTestData->snr_overall_low = 50;

	_pSNRTestData->signal_zone_high = 4000;
	_pSNRTestData->signal_zone_low = 800;
	_pSNRTestData->noise_zone_high = 10;
	_pSNRTestData->noise_zone_low = 0;
	_pSNRTestData->snr_zone_high = 1200;
	_pSNRTestData->snr_zone_low = 50;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();
}

void Ts_BravoSNRTest::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	AcqImageNoFingerTestData *pAcqImageNoFingerTestData = static_cast<AcqImageNoFingerTestData*>(RetrieveTestData("AcqImgNoFinger"));
	if (NULL == pAcqImageNoFingerTestData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoSNRTest::AcqImgNoFinger is not excuted!");
		throw Exception;
		return;
	}
	AcqImageFingerTestData *pAcqImageFingerTestData = static_cast<AcqImageFingerTestData*>(RetrieveTestData("AcqImgFinger"));
	if (NULL == pAcqImageFingerTestData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoSNRTest::AcqImgFinger is not excuted!");
		throw Exception;
		return;
	}

	_pSNRTestData->executed = true;

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;
	unsigned int frameNumbers = pAcqImageNoFingerTestData->numFrame;
	
	int16_t *arr16bitsFrameNoFingerAll = new int16_t[rowNumber*columnNumber * frameNumbers];
	int16_t *arr16bitsFrameFingerAll = new int16_t[rowNumber*columnNumber * frameNumbers];
	for (unsigned int i = 0; i < frameNumbers; i++)
	{
		memcpy(&(arr16bitsFrameNoFingerAll[i*rowNumber*columnNumber]), pAcqImageNoFingerTestData->ListOfImageNoFinger[i]->arrImage, rowNumber*columnNumber*sizeof(int16_t));
		memcpy(&(arr16bitsFrameFingerAll[i*rowNumber*columnNumber]), pAcqImageFingerTestData->ListOfImageFinger[i]->arrImage, rowNumber*columnNumber*sizeof(int16_t));
	}

	try
	{
		rc = synSNRTest(arr16bitsFrameNoFingerAll, arr16bitsFrameFingerAll, rowNumber, columnNumber, frameNumbers, 0, _pSNRTestData->signal_value, _pSNRTestData->noise_value, _pSNRTestData->snr_value);
		//rc = synSNRTest(pAcqImageNoFingerTestData->arrImage, pAcqImageFingerTestData->arrImage, rowNumber, columnNumber, 1, 0, _pSNRTestData->signal_value, _pSNRTestData->noise_value, _pSNRTestData->snr_value);
	}
	catch (...)
	{
		delete[] arr16bitsFrameNoFingerAll;
		arr16bitsFrameNoFingerAll = NULL;
		delete[] arr16bitsFrameFingerAll;
		arr16bitsFrameFingerAll = NULL;
		Exception.SetError(ERROR_TSETSTEP_ARGSLENGTH);
		Exception.SetDescription("synSNRTest is failed!");
		throw Exception;
		return;
	}

	delete[] arr16bitsFrameNoFingerAll;
	arr16bitsFrameNoFingerAll = NULL;
	delete[] arr16bitsFrameFingerAll;
	arr16bitsFrameFingerAll = NULL;

	bool result = true;

	for (unsigned i = 0; i < REGIONS; i++)
	{
		if (i != REGIONS - 1)
		{
			if (_pSNRTestData->signal_value[i]<_pSNRTestData->signal_zone_low || _pSNRTestData->signal_value[i]>_pSNRTestData->signal_zone_high ||
				_pSNRTestData->noise_value[i]<_pSNRTestData->noise_zone_low || _pSNRTestData->noise_value[i]>_pSNRTestData->noise_zone_high ||
				_pSNRTestData->snr_value[i]<_pSNRTestData->snr_zone_low || _pSNRTestData->snr_value[i]>_pSNRTestData->snr_zone_high)
			{
				result = false;
			}
		}
		else
		{
			if (_pSNRTestData->signal_value[i]<_pSNRTestData->signal_overall_low || _pSNRTestData->signal_value[i]>_pSNRTestData->signal_overall_high ||
				_pSNRTestData->noise_value[i]<_pSNRTestData->noise_overall_low || _pSNRTestData->noise_value[i]>_pSNRTestData->noise_overall_high ||
				_pSNRTestData->snr_value[i]<_pSNRTestData->snr_overall_low || _pSNRTestData->snr_value[i]>_pSNRTestData->snr_overall_high)
			{
				result = false;
			}
		}
	}

	_pSNRTestData->pass = result;
}

void Ts_BravoSNRTest::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pSNRTestData->data_name, _pSNRTestData->pass ? "Pass" : "Fail"));
}

void Ts_BravoSNRTest::CleanUp()
{
	CalculateTestTime(_pSNRTestData->test_time);
	if (!(_pSNRTestData->pass))
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("119");

	StoreTestData(_pSNRTestData->data_name, static_cast<SynTestData*>(_pSNRTestData));
}
