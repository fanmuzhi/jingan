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
	if (0 == iListSize)
	{
		Exception.SetError(ERROR_TSETSTEP_ARGSLENGTH);
		Exception.SetDescription("BravoProgrammingIOTA::listOfArgValue length is error!");
		throw Exception;
		return;
	}

	_pProgrammingIOTATestData = new ProgrammingIOTATestData();
	_pProgrammingIOTATestData->IOTAType = std::stoi(listOfArgValue[0]);
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

	if (0 == _pProgrammingIOTATestData->IOTAType)
	{
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
	}
	else
	{
		uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
		uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;

		rc = _pSynModule->FpLoadPatch(IOTAWritePatch.patchbufp, IOTAWritePatch.patchbuflen);
		if (0 != rc)
		{
			Exception.SetError(rc);
			Exception.SetDescription("BravoProgrammingIOTA::FpLoadPatch(iotawrite) is failed!");
			throw Exception;
			return;
		}

		//FW_BL and LNA_BL
		CalibrateTestData *pCalibrateData = static_cast<CalibrateTestData*>(RetrieveTestData("Calibrate"));
		if (NULL == pCalibrateData)
		{
			Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
			Exception.SetDescription("BravoProgrammingIOTA::RetrieveTestData(Calibrate) is failed!");
			throw Exception;
			return;
		}

		//add tag : fw_bl && lna_bl 
		vcsfw_frame_tag_t FrameBase_FW_BL_tag;
		FrameBase_FW_BL_tag.nwords = (rowNumber * columnNumber) >> 1;
		FrameBase_FW_BL_tag.flags = 0x20;
		FrameBase_FW_BL_tag.tagid = VCSFW_FRAME_TAG_FW_BL;
		uint8_t *arr_FW_BL = new uint8_t[rowNumber * columnNumber * 2 + sizeof(vcsfw_frame_tag_t)];
		memcpy(arr_FW_BL, &FrameBase_FW_BL_tag, sizeof(vcsfw_frame_tag_t));
		memcpy(&(arr_FW_BL[sizeof(vcsfw_frame_tag_t)]), &(pCalibrateData->FWBaseline), rowNumber*columnNumber * 2);
		rc = _pSynModule->FpIotawrite(arr_FW_BL, rowNumber * columnNumber * 2 + sizeof(vcsfw_frame_tag_t), VCSFW_IOTA_ITYPE_FRAME_BASE);
		delete[] arr_FW_BL;
		arr_FW_BL = NULL;
		if (0 != rc)
		{
			Exception.SetError(rc);
			Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(FW_BL) is failed!");
			throw Exception;
			return;
		}

		vcsfw_frame_tag_t FrameBase_LNA_BL_tag;
		FrameBase_LNA_BL_tag.nwords = (rowNumber * columnNumber) >> 1;
		FrameBase_LNA_BL_tag.flags = 0x20;
		FrameBase_LNA_BL_tag.tagid = VCSFW_FRAME_TAG_LNA_BL;
		uint8_t *arr_LNA_BL = new uint8_t[rowNumber*columnNumber * 2 + sizeof(vcsfw_frame_tag_t)];
		memcpy(arr_LNA_BL, &FrameBase_LNA_BL_tag, sizeof(vcsfw_frame_tag_t));
		memcpy(&(arr_LNA_BL[sizeof(vcsfw_frame_tag_t)]), &(pCalibrateData->LNABaseline), rowNumber * columnNumber * 2);
		rc = _pSynModule->FpIotawrite(arr_LNA_BL, rowNumber * columnNumber * 2 + sizeof(vcsfw_frame_tag_t), VCSFW_IOTA_ITYPE_FRAME_BASE);
		delete[] arr_LNA_BL;
		arr_LNA_BL = NULL;
		if (0 != rc)
		{
			Exception.SetError(rc);
			Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(LNA_BL) is failed!");
			throw Exception;
			return;
		}
	}

	_pSynModule->FpExitBLMode();

	_pProgrammingIOTATestData->pass = true;
}

void Ts_BravoProgrammingIOTA::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pProgrammingIOTATestData->data_name, _pProgrammingIOTATestData->pass ? "Pass" : "Fail"));
}

void Ts_BravoProgrammingIOTA::CleanUp()
{
	CalculateTestTime(_pProgrammingIOTATestData->test_time);

	StoreTestData(_pProgrammingIOTATestData->data_name, static_cast<SynTestData*>(_pProgrammingIOTATestData));
}