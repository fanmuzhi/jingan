#include "Ts_BravoProgrammingIOTA.h"

Ts_BravoProgrammingIOTA::Ts_BravoProgrammingIOTA(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pProgrammingIOTATestData(NULL)
{
}

Ts_BravoProgrammingIOTA::~Ts_BravoProgrammingIOTA()
{
}

void Ts_BravoProgrammingIOTA::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoProgrammingIOTA::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoProgrammingIOTA::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();
	
	_pProgrammingIOTATestData = new ProgrammingIOTATestData();
	_pProgrammingIOTATestData->data_name = _strName;
}

void Ts_BravoProgrammingIOTA::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	Syn_Patch_Info IOTAWritePatch;
	if (!_pSynDutUtils->Config_MT_Info.GetPatchInfo("iotawrite", IOTAWritePatch))
	{
		Exception.SetError(ERROR_CONFIG_DATA_LOST);
		Exception.SetDescription("BravoProgrammingIOTA::IOTAWritePatch not exists!");
		throw Exception;
		return;
	}
	if (NULL == IOTAWritePatch.patchbufp || 0 == IOTAWritePatch.patchbuflen)
	{
		Exception.SetError(ERROR_CONFIG_DATA_LOST);
		Exception.SetDescription("BravoProgrammingIOTA::IOTAWritePatch has no value!");
		throw Exception;
		return;
	}

	//BootLoader Mode
	_pSynModule->FpEnterBLMode();
	bool InBootloaderMode(false);
	rc = _pSynModule->checkBLMode(InBootloaderMode);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoProgrammingIOTA::checkBLMode is failed!");
		throw Exception;
		return;
	}
	if (!InBootloaderMode)
	{
		Exception.SetError(ERROR_BOOTLOADERMODE_CHECK);
		Exception.SetDescription("BravoProgrammingIOTA::InBootloaderMode is false!");
		throw Exception;
		return;
	}

	_pProgrammingIOTATestData->executed = true;

	//erase
	Syn_Patch_Info FlashErasePatch;
	_pSynDutUtils->Config_MT_Info.GetPatchInfo("flasherase", FlashErasePatch);
	if (NULL == FlashErasePatch.patchbufp || 0 == FlashErasePatch.patchbuflen)
	{
		Exception.SetError(ERROR_CONFIG_DATA_LOST);
		Exception.SetDescription("BravoProgrammingIOTA::flasherase is NULL!");
		throw Exception;
		return;
	}
	rc = _pSynModule->FpLoadPatch(FlashErasePatch.patchbufp, FlashErasePatch.patchbuflen);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoProgrammingIOTA::FpLoadPatch is failed!");
		throw Exception;
		return;
	}
	rc = _pSynModule->FpFlashErase(FlashEraseType::IOTA);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoProgrammingIOTA::FpFlashErase is failed!");
		throw Exception;
		return;
	}
	_pSynModule->FpReset();

	rc = _pSynModule->FpLoadPatch(IOTAWritePatch.patchbufp, IOTAWritePatch.patchbuflen);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoProgrammingIOTA::FpLoadPatch(iotawrite) is failed!");
		throw Exception;
		return;
	}

	//IOTA bin file data
	//BASE.bin, VERSION.bin, PSELECT.bin
	Syn_Patch_Info IOTABasePatch, VERSIONPatch, PSELECTPatch;
	if (!_pSynDutUtils->Config_MT_Info.GetPatchInfo("iotabase", IOTABasePatch))
	{
		Exception.SetError(ERROR_CONFIG_DATA_LOST);
		Exception.SetDescription("BravoProgrammingIOTA::IOTAWritePatch not exists!");
		throw Exception;
		return;
	}

	_pSynDutUtils->Config_MT_Info.GetPatchInfo("iotaconfigversion", VERSIONPatch);
	_pSynDutUtils->Config_MT_Info.GetPatchInfo("iotapselect", PSELECTPatch);

	rc = _pSynModule->FpIotawrite(IOTABasePatch.patchbufp, IOTABasePatch.patchbuflen, 0);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(BASE) is failed!");
		throw Exception;
		return;
	}

	if (NULL != VERSIONPatch.patchbufp && 0 != VERSIONPatch.patchbuflen)
	{
		rc = _pSynModule->FpIotawrite(VERSIONPatch.patchbufp, VERSIONPatch.patchbuflen, 0);
		if (0 != rc)
		{
			Exception.SetError(rc);
			Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(VERSION) is failed!");
			throw Exception;
			return;
		}
	}
	if (NULL != PSELECTPatch.patchbufp && 0 != PSELECTPatch.patchbuflen)
	{
		rc = _pSynModule->FpIotawrite(PSELECTPatch.patchbufp, PSELECTPatch.patchbuflen, 0);
		if (0 != rc)
		{
			Exception.SetError(rc);
			Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(PSELECT) is failed!");
			throw Exception;
			return;
		}
	}

	////VCSFW_IOTA_ITYPE_CONFIG_FRAME
	//vcsfw_config_tag_frame_avg_t config_tag_frame_avg;
	//config_tag_frame_avg.frame_avg = 2;
	//config_tag_frame_avg.unused[0] = 0;
	//config_tag_frame_avg.unused[1] = 0;
	//config_tag_frame_avg.unused[2] = 0;
	//rc = _pSynModule->FpIotawrite((uint8_t*)&config_tag_frame_avg, sizeof(vcsfw_config_tag_frame_avg_t), VCSFW_IOTA_ITYPE_CONFIG_FRAME_AVG);
	//if (0 != rc)
	//{
	//	Exception.SetError(rc);
	//	Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(VCSFW_IOTA_ITYPE_CONFIG_FRAME) is failed!");
	//	throw Exception;
	//	return;
	//}
	
	_pSynModule->FpExitBLMode();

	_pProgrammingIOTATestData->pass = true;
}

void Ts_BravoProgrammingIOTA::ProcessData()
{
	if (!_pProgrammingIOTATestData->pass)
	{
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("153");
	}
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pProgrammingIOTATestData->data_name, _pProgrammingIOTATestData->pass ? "Pass" : "Fail"));
}

void Ts_BravoProgrammingIOTA::CleanUp()
{
	CalculateTestTime(_pProgrammingIOTATestData->test_time);

	StoreTestData(_pProgrammingIOTATestData->data_name, static_cast<SynTestData*>(_pProgrammingIOTATestData));
}