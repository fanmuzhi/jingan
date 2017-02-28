#include "Ts_BravoWOF_Signal.h"
#include "vcsfw\wofplot.h"

Ts_BravoWOF_Signal::Ts_BravoWOF_Signal(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
: Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pWOF_SignalTestData(NULL)
{
}

Ts_BravoWOF_Signal::~Ts_BravoWOF_Signal()
{
}

void Ts_BravoWOF_Signal::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoWOF_Signal::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoWOF_Signal::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pWOF_SignalTestData = new WOF_SignalTestData();
	_pWOF_SignalTestData->data_name = _strName;
	_pWOF_SignalTestData->lowSignalLimit = 40;
	_pWOF_SignalTestData->highSignalLimit = 70;
	_pWOF_SignalTestData->lowDCAOffsetLimit = 30;
	_pWOF_SignalTestData->highDCAOffsetLimit = 70;

	_pWOF_SignalTestData->selectedGain = 0;
	_pWOF_SignalTestData->selectedOffset = 0;
	_pWOF_SignalTestData->selectedSingal = 0;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);
	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

	if (iListSize < 4)
	{
		for (size_t t = 1; t <= 4 - iListSize; t++)
			listOfArgValue.push_back("");
	}

	if (0 != listOfArgValue[0].length())
		_pWOF_SignalTestData->lowSignalLimit = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pWOF_SignalTestData->highSignalLimit = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pWOF_SignalTestData->lowDCAOffsetLimit = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pWOF_SignalTestData->highDCAOffsetLimit = atoi(listOfArgValue[3].c_str());

}

void Ts_BravoWOF_Signal::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	//get wofsignal patch(same as wofbaseline patch)
	Syn_Patch_Info WOFSignalPatch;
	if (!_pSynDutUtils->Config_MT_Info.GetPatchInfo("wofsignal", WOFSignalPatch))
	{
		Exception.SetError(ERROR_CONFIG_ERR);
		Exception.SetDescription("BravoWOF_Signal::Can not get wofsignal patch!");
		throw Exception;
		return;
	}
	if (NULL == WOFSignalPatch.patchbufp || 0 == WOFSignalPatch.patchbuflen)
	{
		Exception.SetError(ERROR_CONFIG_DATA_LOST);
		Exception.SetDescription("BravoWOF_Signal::wofsignal patch is NULL!");
		throw Exception;
		return;
	}

	//Retrieve wofbaseline data
	WOF_BaselineTestData *pWOF_BaselineTestData = static_cast<WOF_BaselineTestData*>(RetrieveTestData("WOF_Baseline"));
	if (NULL == pWOF_BaselineTestData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoWOF_Signal::Can not retrieve WOF_BaselineTestData!");
		throw Exception;
		return;
	}

	//Enter bootloader mode
	_pSynModule->FpEnterBLMode();
	bool IsBootloaderMode(false);
	rc = _pSynModule->checkBLMode(IsBootloaderMode);
	if (0 != rc)
	{
		Exception.SetError(rc);
		throw Exception;
		return;
	}
	if (!IsBootloaderMode)
	{
		Exception.SetError(ERROR_BOOTLOADERMODE_CHECK);
		Exception.SetDescription("BravoWOF_Signal::Can not enter bootloader mode!");
		throw Exception;
		return;
	}

	rc = _pSynModule->FpLoadPatch(WOFSignalPatch.patchbufp, WOFSignalPatch.patchbuflen);
	if (0 != rc)
	{
		Exception.SetError(rc);
		throw Exception;
		return;
	}

	//get baseline data
	uint8_t arrRawOffsets[BRAVO_WOF_DATA_MAX] = { 0 };
	for (unsigned int i = 0; i < BRAVO_WOF_COUNTS_MAX; i++)
	{
		memcpy(&(arrRawOffsets[i * 4]), &(pWOF_BaselineTestData->arrWOFBaseline[i]), sizeof(uint16_t));
		memcpy(&(arrRawOffsets[i * 4 + sizeof(uint16_t)]), &(pWOF_BaselineTestData->arrWOFOffset[i]), sizeof(uint16_t));
	}
	uint8_t arrRawSignals[BRAVO_WOF_DATA_MAX] = { 0 };

	rc = _pSynModule->FpWOFSignal(arrRawOffsets, BRAVO_WOF_DATA_MAX, arrRawSignals, BRAVO_WOF_DATA_MAX);
	if (0 != rc)
	{
		Exception.SetError(rc);
		throw Exception;
		return;
	}

	rc = _pSynModule->FpExitBLMode();

	memcpy(_pWOF_SignalTestData->arrWOFSignal, arrRawSignals, sizeof(uint16_t) * BRAVO_WOF_COUNTS_MAX);
	for (unsigned int i = 0; i < BRAVO_WOF_COUNTS_MAX; i++)
	{
		_pWOF_SignalTestData->arrWOFSignal[i] = WOF_BASELINE - _pWOF_SignalTestData->arrWOFSignal[i];
	}

	_pWOF_SignalTestData->executed = true;

	//find signal
	
	int signalPosition = -1;
	uint16_t guardSignalValue = 0;
	for (unsigned int i = 0; i < BRAVO_WOF_COUNTS_MAX; i++)
	{
		uint16_t currentSignal = _pWOF_SignalTestData->arrWOFSignal[i];
		uint16_t currentOffset = pWOF_BaselineTestData->arrWOFOffset[i];

		if ((_pWOF_SignalTestData->lowSignalLimit < currentSignal) &&
			(currentSignal < _pWOF_SignalTestData->highSignalLimit) &&
			(_pWOF_SignalTestData->lowDCAOffsetLimit < currentOffset) &&
			(currentOffset < _pWOF_SignalTestData->highDCAOffsetLimit)
			)
		{
			if (currentSignal>guardSignalValue)
			{
				guardSignalValue = currentSignal;
				signalPosition = i;
			}
		}
	}

	if (0 <= signalPosition)
	{
		_pWOF_SignalTestData->pass = true;

		_pWOF_SignalTestData->selectedGain = pWOF_BaselineTestData->arrWOFBaseline[signalPosition];
		_pWOF_SignalTestData->selectedOffset = pWOF_BaselineTestData->arrWOFOffset[signalPosition];
		_pWOF_SignalTestData->selectedSingal = _pWOF_SignalTestData->arrWOFSignal[signalPosition];
	}
	else
	{
		_pWOF_SignalTestData->pass = false;
	}

}

void Ts_BravoWOF_Signal::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pWOF_SignalTestData->data_name, _pWOF_SignalTestData->pass ? "Pass" : "Fail"));
	if (!(_pWOF_SignalTestData->pass))
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("131");
}

void Ts_BravoWOF_Signal::CleanUp()
{
	CalculateTestTime(_pWOF_SignalTestData->test_time);

	StoreTestData(_pWOF_SignalTestData->data_name, static_cast<SynTestData*>(_pWOF_SignalTestData));
}