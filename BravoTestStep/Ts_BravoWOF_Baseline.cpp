#include "Ts_BravoWOF_Baseline.h"
#include "vcsfw\wofplot.h"

Ts_BravoWOF_Baseline::Ts_BravoWOF_Baseline(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
: Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pWOF_BaselineTestData(NULL)
{
}

Ts_BravoWOF_Baseline::~Ts_BravoWOF_Baseline()
{
}

void Ts_BravoWOF_Baseline::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoWOF_Baseline::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoWOF_Baseline::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pWOF_BaselineTestData = new WOF_BaselineTestData();
	_pWOF_BaselineTestData->data_name = _strName;
	_pWOF_BaselineTestData->gainNumbers = 5;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);
	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

}

void Ts_BravoWOF_Baseline::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	//get wofbaseline patch(same as wofsignal patch)
	Syn_Patch_Info WofBaselinePatch;
	if (!_pSynDutUtils->Config_MT_Info.GetPatchInfo("wofbaseline", WofBaselinePatch))
	{
		Exception.SetError(ERROR_CONFIG_ERR);
		Exception.SetDescription("BravoWOF_Baseline::Can not get wofbaseline patch!");
		throw Exception;
		return;
	}
	if (NULL == WofBaselinePatch.patchbufp || 0 == WofBaselinePatch.patchbuflen)
	{
		Exception.SetError(ERROR_CONFIG_DATA_LOST);
		Exception.SetDescription("BravoWOF_Baseline::wofbaseline patch is NULL!");
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
		Exception.SetDescription("BravoWOF_Baseline::Can not enter bootloader mode!");
		throw Exception;
		return;
	}

	rc = _pSynModule->FpLoadPatch(WofBaselinePatch.patchbufp, WofBaselinePatch.patchbuflen);
	if (0 != rc)
	{
		Exception.SetError(rc);
		throw Exception;
		return;
	}

	uint8_t arrWOF_Baseline_Offset[BRAVO_WOF_DATA_MAX] = { 0 };
	rc = _pSynModule->FpWOFBaseline(arrWOF_Baseline_Offset, BRAVO_WOF_DATA_MAX);
	if (0 != rc)
	{
		Exception.SetError(rc);
		throw Exception;
		return;
	}

	rc = _pSynModule->FpExitBLMode();

	for (unsigned int i = 0; i < BRAVO_WOF_COUNTS_MAX; i++)
	{
		memcpy(&(_pWOF_BaselineTestData->arrWOFBaseline[i]), &(arrWOF_Baseline_Offset[i * BRAVO_WOF_DATA_MAX / BRAVO_WOF_COUNTS_MAX]), sizeof(uint16_t));
		memcpy(&(_pWOF_BaselineTestData->arrWOFOffset[i]), &(arrWOF_Baseline_Offset[(i * BRAVO_WOF_DATA_MAX / BRAVO_WOF_COUNTS_MAX) + sizeof(uint16_t)]), sizeof(uint16_t));
	}

	_pWOF_BaselineTestData->executed = true;

	_pWOF_BaselineTestData->pass = true;
}

void Ts_BravoWOF_Baseline::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pWOF_BaselineTestData->data_name, _pWOF_BaselineTestData->pass ? "Pass" : "Fail"));
	/*if (!(_pWOF_SignalTestData->pass))
	_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("129");*/
}

void Ts_BravoWOF_Baseline::CleanUp()
{
	CalculateTestTime(_pWOF_BaselineTestData->test_time);

	StoreTestData(_pWOF_BaselineTestData->data_name, static_cast<SynTestData*>(_pWOF_BaselineTestData));
}