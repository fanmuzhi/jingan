#include "Ts_BravoAcqImgNoFinger.h"

Ts_BravoAcqImgNoFinger::Ts_BravoAcqImgNoFinger(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pAcqImageNoFingerTestData(NULL)
{
}

Ts_BravoAcqImgNoFinger::~Ts_BravoAcqImgNoFinger()
{
}

void Ts_BravoAcqImgNoFinger::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoAcqImageNoFinger::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoAcqImageNoFinger::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pAcqImageNoFingerTestData = new AcqImageNoFingerTestData();
	_pAcqImageNoFingerTestData->data_name = _strName;
	_pAcqImageNoFingerTestData->numFrame = 30;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();
	if (0 != iListSize)
	{
		_pAcqImageNoFingerTestData->numFrame = std::stoi(listOfArgValue[0]);
	}
}

void Ts_BravoAcqImgNoFinger::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	bool CalibrateExcuted(false);
	for (size_t t = 0; t < _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
	{
		SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t];
		if (NULL != Test_data)
		{
			if ("Calibrate" == Test_data->data_name)
			{
				CalibrateExcuted = true;
				break;
			}
		}
	}
	if (!CalibrateExcuted)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoAcqImageNoFinger::Calibrate is not excuted!");
		throw Exception;
		return;
	}

	bool IsPass = false;
	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;
	for (unsigned int i = 0; i < _pAcqImageNoFingerTestData->numFrame; i++)
	{
		uint8_t *arr_8bitsFrame = new uint8_t[rowNumber * columnNumber * 2];
		rc = _pSynModule->FpFrameRead(arr_8bitsFrame, rowNumber * columnNumber * 2);
		_pSynModule->FpFrameFinish();
		if (0 == rc)
		{
			SimpleFpFrame *pSimpleFpFrame = new SimpleFpFrame();
			memcpy(pSimpleFpFrame->arrImage, arr_8bitsFrame, rowNumber * columnNumber * 2);
			delete[] arr_8bitsFrame;
			arr_8bitsFrame = NULL;

			_pAcqImageNoFingerTestData->ListOfImageNoFinger.push_back(pSimpleFpFrame);
			IsPass = true;
		}
	}

	_pAcqImageNoFingerTestData->executed = true;
	_pAcqImageNoFingerTestData->pass = IsPass;

	unsigned int currentSize = _pAcqImageNoFingerTestData->ListOfImageNoFinger.size();
	//caculate avg
	int32_t temp = 0;
	for (unsigned int i = 0; i < rowNumber*columnNumber; i++)
	{
		for (unsigned int j = 0; j < currentSize; j++)
		{
			temp += _pAcqImageNoFingerTestData->ListOfImageNoFinger[j]->arrImage[i];
		}

		int16_t result = (double)temp / currentSize;
		if (result > 32767 || result < -32768)
			result = result > 32767 ? (int16_t)32767 : (int16_t)-32768;

		_pAcqImageNoFingerTestData->arrImage[i] = result;
		temp = 0;
	}
}

void Ts_BravoAcqImgNoFinger::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pAcqImageNoFingerTestData->data_name, _pAcqImageNoFingerTestData->pass ? "Pass" : "Fail"));
}

void Ts_BravoAcqImgNoFinger::CleanUp()
{
	CalculateTestTime(_pAcqImageNoFingerTestData->test_time);

	StoreTestData(_pAcqImageNoFingerTestData->data_name, static_cast<SynTestData*>(_pAcqImageNoFingerTestData));
}