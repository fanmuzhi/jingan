#include "Ts_BravoHuaweiImageTest.h"

typedef float(_stdcall *snrTest)(unsigned char*, unsigned char*, int, int, int, int *, float *, float fcrop);

Ts_BravoHuaweiImageTest::Ts_BravoHuaweiImageTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pHuaweiImageTestData(NULL)
{
}

Ts_BravoHuaweiImageTest::~Ts_BravoHuaweiImageTest()
{
}

void Ts_BravoHuaweiImageTest::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoHuaweiImageTest::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoHuaweiImageTest::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pHuaweiImageTestData = new HuaweiImageTestData();
	_pHuaweiImageTestData->data_name = _strName;
	_pHuaweiImageTestData->snrLimit = 35;
	_pHuaweiImageTestData->signalLimit = 0;
	_pHuaweiImageTestData->noiseLimit = 0;

	_pHuaweiImageTestData->snrValue = 0;
	_pHuaweiImageTestData->signalValue = 0;
	_pHuaweiImageTestData->noiseValue = 0;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();
	if (0 != iListSize)
	{
		_pHuaweiImageTestData->snrLimit = std::stoi(listOfArgValue[0]);
	}
}

void Ts_BravoHuaweiImageTest::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	AcqImageNoFingerTestData *pAcqImageNoFingerTestData = static_cast<AcqImageNoFingerTestData*>(RetrieveTestData("AcqImgNoFinger"));
	if (NULL == pAcqImageNoFingerTestData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoHuaweiImageTest::AcqImgNoFinger is not excuted!");
		throw Exception;
		return;
	}
	AcqImageFingerTestData *pAcqImageFingerTestData = static_cast<AcqImageFingerTestData*>(RetrieveTestData("AcqImgFinger"));
	if (NULL == pAcqImageFingerTestData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoHuaweiImageTest::AcqImgFinger is not excuted!");
		throw Exception;
		return;
	}

	float snrValue = 0;
	int signalValue = 0;
	float noiseValue = 0;

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;

	unsigned int frameNumbers = pAcqImageNoFingerTestData->numFrame;

	uint8_t *arr8bitsFrameNoFingerAll = new uint8_t[rowNumber*columnNumber * frameNumbers];
	uint8_t *arr8bitsFrameFingerAll = new uint8_t[rowNumber*columnNumber * frameNumbers];
	for (unsigned int i = 0; i < frameNumbers; i++)
	{
		uint8_t *arr8bitFrameNoFinger = new uint8_t[rowNumber*columnNumber];
		uint8_t *arr8bitFrameFinger = new uint8_t[rowNumber*columnNumber];

		bpp16tobpp8(pAcqImageNoFingerTestData->ListOfImageNoFinger[i]->arrImage, arr8bitFrameNoFinger, rowNumber, columnNumber);
		bpp16tobpp8(pAcqImageFingerTestData->ListOfImageFinger[i]->arrImage, arr8bitFrameFinger, rowNumber, columnNumber);

		memcpy(&(arr8bitsFrameNoFingerAll[i*rowNumber*columnNumber]), arr8bitFrameNoFinger, rowNumber*columnNumber);
		memcpy(&(arr8bitsFrameFingerAll[i*rowNumber*columnNumber]), arr8bitFrameFinger, rowNumber*columnNumber);

		delete[] arr8bitFrameNoFinger;
		arr8bitFrameNoFinger = NULL;
		delete[] arr8bitFrameFinger;
		arr8bitFrameFinger = NULL;
	}

	HINSTANCE hdll = NULL;
	snrTest snrTestFunc;
	//use huawei snr as temp, waitng for new snr algorithm release
	HINSTANCE hDLL = LoadLibrary(TEXT("snrtest.dll"));
	if (NULL != hDLL)
	{
		snrTestFunc = (snrTest)GetProcAddress(hDLL, "testSNR");
		if (NULL != snrTestFunc)
		{
			try
			{
				snrValue = snrTestFunc(arr8bitsFrameNoFingerAll, arr8bitsFrameFingerAll, columnNumber, rowNumber, frameNumbers, &signalValue, &noiseValue, 0.2f);
			}
			catch (...)
			{
				Exception.SetError(ERROR_TSETSTEP_ARGSLENGTH);
				Exception.SetDescription("testSNR is failed!");
				throw Exception;
				return;
			}
		}

		FreeLibrary(hdll);
		hdll = NULL;
	}

	delete[] arr8bitsFrameNoFingerAll;
	arr8bitsFrameNoFingerAll = NULL;
	delete[] arr8bitsFrameFingerAll;
	arr8bitsFrameFingerAll = NULL;

	_pHuaweiImageTestData->executed = true;
	if (snrValue > _pHuaweiImageTestData->snrLimit)
	{
		_pHuaweiImageTestData->pass = true;
	}
	else
	{
		_pHuaweiImageTestData->pass = false;
	}

	_pHuaweiImageTestData->snrValue = snrValue;
	_pHuaweiImageTestData->signalValue = signalValue;
	_pHuaweiImageTestData->noiseValue = noiseValue;
}

void Ts_BravoHuaweiImageTest::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pHuaweiImageTestData->data_name, _pHuaweiImageTestData->pass ? "Pass" : "Fail"));
}

void Ts_BravoHuaweiImageTest::CleanUp()
{
	CalculateTestTime(_pHuaweiImageTestData->test_time);
	if (!(_pHuaweiImageTestData->pass))
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("119");

	StoreTestData(_pHuaweiImageTestData->data_name, static_cast<SynTestData*>(_pHuaweiImageTestData));
}
