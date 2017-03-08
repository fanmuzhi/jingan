#include "Ts_BravoSharpness.h"
#include "synImageTest.h"

//sharpness test kernels
//static int m_kernelSharpnessGx[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };
//static int m_kernelSharpnessGy[3][3] = { { 1, 0, -1 }, { 2, 0, -2 }, { 1, 0, -1 } };

Ts_BravoSharpness::Ts_BravoSharpness(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
{
}


Ts_BravoSharpness::~Ts_BravoSharpness()
{
}

void Ts_BravoSharpness::SetUp()
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

	_pSharpnessData = new SharpnessData();
	_pSharpnessData->data_name = _strName;
	_pSharpnessData->limit = 30;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();
	if (0 != iListSize)
	{
		_pSharpnessData->limit = std::stoi(listOfArgValue[0]);
	}
}

void Ts_BravoSharpness::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;

	//get 8bit images
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

	unsigned int frameNumbers = pAcqImageNoFingerTestData->numFrame;
	
	uint8_t *arr8bitsFrameFinger = new uint8_t[rowNumber*columnNumber];
	bpp16tobpp8(pAcqImageFingerTestData->arrImage, arr8bitsFrameFinger, rowNumber, columnNumber);

	try
	{
		rc = synSharpnessTest(arr8bitsFrameFinger, rowNumber, columnNumber, _pSharpnessData->Gx, _pSharpnessData->Gy, _pSharpnessData->h, _pSharpnessData->g, _pSharpnessData->pImg_t, _pSharpnessData->SHARPNESS, &(_pSharpnessData->percent));
	}
	catch (...)
	{
		delete[] arr8bitsFrameFinger;
		arr8bitsFrameFinger = NULL;
		Exception.SetError(ERROR_TSETSTEP_ARGSLENGTH);
		Exception.SetDescription("synSharpnessTest is failed!");
		throw Exception;
		return;
	}

	delete[] arr8bitsFrameFinger;
	arr8bitsFrameFinger = NULL;

	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("synSharpnessTest is failed!");
		throw Exception;
		return;
	}

	_pSharpnessData->executed = true;
}

void Ts_BravoSharpness::ProcessData()
{
	if (_pSharpnessData->percent > _pSharpnessData->limit)
	{
		_pSharpnessData->pass = false;
	}
	else
	{
		_pSharpnessData->pass = true;
	}

	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pSharpnessData->data_name, _pSharpnessData->pass ? "Pass" : "Fail"));

}

void Ts_BravoSharpness::CleanUp()
{
	CalculateTestTime(_pSharpnessData->test_time);
	if (!(_pSharpnessData->pass))
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("126");

	StoreTestData(_pSharpnessData->data_name, static_cast<SynTestData*>(_pSharpnessData));

}