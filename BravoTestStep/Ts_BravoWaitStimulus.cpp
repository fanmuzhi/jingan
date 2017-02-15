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

	bool waitStilumusExcuted(false);
	for (size_t t = 0; t < _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
	{
		SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t];
		if (NULL != Test_data)
		{
			if ("WaitStimulus" == Test_data->data_name)
			{
				waitStilumusExcuted = true;
				break;
			}
		}
	}

	if (!waitStilumusExcuted)
	{
		_WaitStilumusTestData = new WaitStilumusTestData();
		_WaitStilumusTestData->data_name = _strName;
		SynTestData *pSynTestData = static_cast<SynTestData*>(_WaitStilumusTestData);
		_pSynDutUtils->_pDutTestResult->list_testdata.push_back(pSynTestData);
	}
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

	bool waitStilumusExists(false);
	for (size_t t = 0; t < _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
	{
		SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t];
		if (NULL != Test_data)
		{
			if ("WaitStimulus" == Test_data->data_name)
			{
				waitStilumusExists = true;
				_WaitStilumusTestData = static_cast<WaitStilumusTestData*>(_pSynDutUtils->_pDutTestResult->list_testdata[t]);
				break;
			}
		}
	}
	if (NULL == _WaitStilumusTestData)
	{
		_WaitStilumusTestData = new WaitStilumusTestData();
		_WaitStilumusTestData->data_name = _strName;
		SynTestData *pSynTestData = static_cast<SynTestData*>(_WaitStilumusTestData);
		_pSynDutUtils->_pDutTestResult->list_testdata.push_back(pSynTestData);
	}

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;
	uint8_t *arrFrame = new uint8_t[rowNumber*columnNumber*2];
	rc = _pSynModule->FpFrameRead(arrFrame, rowNumber * columnNumber * 2);
	if (0 == rc)
	{
		memcpy(_WaitStilumusTestData->FrameData, arrFrame, rowNumber*columnNumber * 2);
		for (size_t t = 0; t < _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
		{
			SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t];
			if (NULL != Test_data)
			{
				if ("WaitStimulus" == Test_data->data_name)
				{
					waitStilumusExists = true;
					SynTestData *pSynTestData = static_cast<WaitStilumusTestData*>(_WaitStilumusTestData);
					_pSynDutUtils->_pDutTestResult->list_testdata[t] = pSynTestData;
					break;
				}
			}
		}
	}

	delete[] arrFrame;
	arrFrame = NULL;
}

void Ts_BravoWaitStimulus::ProcessData()
{
}

void Ts_BravoWaitStimulus::CleanUp()
{
	_WaitStilumusTestData->test_time = 0;

	bool waitStilumusExists(false);
	for (size_t t = 0; t < _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
	{
		SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t];
		if (NULL != Test_data)
		{
			if ("WaitStimulus" == Test_data->data_name)
			{
				waitStilumusExists = true;
				SynTestData *pSynTestData = static_cast<WaitStilumusTestData*>(_WaitStilumusTestData);
				_pSynDutUtils->_pDutTestResult->list_testdata[t] = pSynTestData;
				break;
			}
		}
	}

	if (!waitStilumusExists)
	{
		SynTestData *pSynTestData = static_cast<SynTestData*>(_WaitStilumusTestData);
		_pSynDutUtils->_pDutTestResult->list_testdata.push_back(pSynTestData);
	}
}