#include "Ts_BravoWaitStimulus.h"

Ts_BravoWaitStimulus::Ts_BravoWaitStimulus(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _WaitStilumusTestData(NULL)
{
}

Ts_BravoWaitStimulus::~Ts_BravoWaitStimulus()
{
}

void Ts_BravoWaitStimulus::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoWaitStimulus::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoWaitStimulus::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

	RetrieveWaitStilumus();
	_WaitStilumusTestData->data_name = _strName;
}

void Ts_BravoWaitStimulus::Execute()
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
		return;

	RetrieveWaitStilumus();

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;
	uint8_t *arrFrame = new uint8_t[rowNumber*columnNumber*2];
	rc = _pSynModule->FpFrameRead(arrFrame, rowNumber * columnNumber * 2);
	_pSynModule->FpFrameFinish();
	if (0 == rc)
	{
		memcpy(_WaitStilumusTestData->FrameData, arrFrame, rowNumber*columnNumber * 2);
		_WaitStilumusTestData->executed = true;
		_WaitStilumusTestData->pass = true;
		StoreWaitStilumus();
	}

	delete[] arrFrame;
	arrFrame = NULL;
}

void Ts_BravoWaitStimulus::ProcessData()
{
}

void Ts_BravoWaitStimulus::CleanUp()
{
	RetrieveWaitStilumus();

	_WaitStilumusTestData->test_time = 0;

	StoreWaitStilumus();
}


void Ts_BravoWaitStimulus::RetrieveWaitStilumus()
{
	bool WaitStilumusExists(false);
	for (size_t t = 0; t < _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
	{
		SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t];
		if (NULL != Test_data)
		{
			if ("WaitStimulus" == Test_data->data_name)
			{
				WaitStilumusExists = true;
				_WaitStilumusTestData = static_cast<WaitStilumusTestData*>(_pSynDutUtils->_pDutTestResult->list_testdata[t]);
				break;
			}
		}
	}

	if (!WaitStilumusExists)
	{
		_WaitStilumusTestData = new WaitStilumusTestData();
		_WaitStilumusTestData->data_name = _strName;
		SynTestData *pSynTestData = static_cast<SynTestData*>(_WaitStilumusTestData);
		_pSynDutUtils->_pDutTestResult->list_testdata.push_back(pSynTestData);
	}
}

void Ts_BravoWaitStimulus::StoreWaitStilumus()
{
	if (NULL == !_WaitStilumusTestData)
	{
		bool WaitStilumusExists(false);
		for (size_t t = 0; t < _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
		{
			SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t];
			if (NULL != Test_data)
			{
				if ("WaitStimulus" == Test_data->data_name)
				{
					WaitStilumusExists = true;
					_pSynDutUtils->_pDutTestResult->list_testdata[t] = static_cast<SynTestData*>(_WaitStilumusTestData);
					break;
				}
			}
		}

		if (!WaitStilumusExists)
		{
			SynTestData *pSynTestData = static_cast<SynTestData*>(_WaitStilumusTestData);
			_pSynDutUtils->_pDutTestResult->list_testdata.push_back(pSynTestData);
		}
	}
}