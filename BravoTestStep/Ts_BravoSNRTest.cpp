#include "Ts_BravoSNRTest.h"
#include "synImageTest.h"

typedef float(_stdcall *snrTest)(unsigned char*, unsigned char*, int, int, int, int *, float *, float fcrop);

static float kernel[9][9] =
{
	{ 0.0000, 0.0000, 0.0001, 0.0005, 0.0007, 0.0005, 0.0001, 0.0000, 0.0000 },
	{ 0.0000, 0.0003, 0.0026, 0.0086, 0.0124, 0.0086, 0.0026, 0.0003, 0.0000 },
	{ 0.0001, 0.0026, 0.0175, 0.0392, 0.0431, 0.0392, 0.0175, 0.0026, 0.0001 },
	{ 0.0005, 0.0086, 0.0392, 0.0000, -0.0965, 0.0000, 0.0392, 0.0086, 0.0005 },
	{ 0.0007, 0.0124, 0.0431, -0.0965, -0.3183, -0.0965, 0.0431, 0.0124, 0.0007 },
	{ 0.0005, 0.0086, 0.0392, 0.0000, -0.0965, 0.0000, 0.0392, 0.0086, 0.0005 },
	{ 0.0001, 0.0026, 0.0175, 0.0392, 0.0431, 0.0392, 0.0175, 0.0026, 0.0001 },
	{ 0.0000, 0.0003, 0.0026, 0.0086, 0.0124, 0.0086, 0.0026, 0.0003, 0.0000 },
	{ 0.0000, 0.0000, 0.0001, 0.0005, 0.0007, 0.0005, 0.0001, 0.0000, 0.0000 }
};

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
	_pSNRTestData->snrLimit = 35;
	_pSNRTestData->signalLimit = 0;
	_pSNRTestData->noiseLimit = 0;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();
	if (0 != iListSize)
	{
		_pSNRTestData->snrLimit = std::stoi(listOfArgValue[0]);
	}
	
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

	_pSNRTestData->snrValue = 0;
	_pSNRTestData->signalValue = 0;
	_pSNRTestData->noiseValue = 0;

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

	_pSNRTestData->executed = true;
	if (snrValue > _pSNRTestData->snrLimit)
	{
		_pSNRTestData->pass = true;
	}
	else
	{
		_pSNRTestData->pass = false;
	}

	//test
	int arrSingal[7] = { 0 };
	float arrNoise[7] = { 0 };
	double arrSNR[7] = { 0 };
	rc = synSNRTest(pAcqImageNoFingerTestData->arrImage, pAcqImageFingerTestData->arrImage, rowNumber, columnNumber, arrSingal, arrNoise, arrSNR);

	_pSNRTestData->snrValue = snrValue;
	_pSNRTestData->signalValue = signalValue;
	_pSNRTestData->noiseValue = noiseValue;
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
