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

	_pProgrammingIOTATestData->executed = true;

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
		//add bincode check
		unsigned int bincodesCounts = _pSynDutUtils->_pDutTestResult->list_bincodes.size();
		if (0 != bincodesCounts)
		{
			_pProgrammingIOTATestData->pass = false;
			return;
		}

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
		if (NULL != pCalibrateData)
		{
			//add tag : fw_bl && lna_bl 
			vcsfw_frame_tag_t FrameBase_FW_BL_tag;
			FrameBase_FW_BL_tag.nwords = (rowNumber * columnNumber * 2) / sizeof(uint32_t);//number of 32-bit to follow
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
			FrameBase_LNA_BL_tag.nwords = (rowNumber * columnNumber * 2) / sizeof(uint32_t);//number of 32-bit to follow
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

		//WOF
		WOF_BaselineTestData *pWOF_BaselineTestData = static_cast<WOF_BaselineTestData*>(RetrieveTestData("WOF_Baseline"));
		WOF_SignalTestData *pWOF_SignalTestData = static_cast<WOF_SignalTestData*>(RetrieveTestData("WOF_Signal"));
		if (NULL != pWOF_BaselineTestData&&NULL != pWOF_SignalTestData)
		{
			uint32_t gainRegister(0), offsetRegister(0), ctrlRegister(0);
			rc = _pSynModule->FpGetWOFRegister(gainRegister, offsetRegister, ctrlRegister);
			if (0 != rc)
			{
				Exception.SetError(rc);
				Exception.SetDescription("BravoProgrammingIOTA::FpGetWOFRegisteris failed!");
				throw Exception;
				return;
			}

			uint32_t gainValue = pWOF_SignalTestData->selectedGain;
			uint16_t offsetValue = pWOF_SignalTestData->selectedOffset;
			uint32_t signalValue = pWOF_SignalTestData->selectedSingal;

			//REG16BLK	WOF_GAIN
			vcsfw_frame_tag_t frame_tag_gain;
			frame_tag_gain.tagid = VCSFW_FRAME_TAG_REG16BLK;
			frame_tag_gain.nwords = 2;
			frame_tag_gain.flags = 0x20;

			vcsfw_frame_tag_reg16blk_t frame_tag_reg16blk_gain;
			frame_tag_reg16blk_gain.regbase = gainRegister;//0x4950 is for shasta, denali is 0x2658
			frame_tag_reg16blk_gain.nregs = 1;

			uint8_t *arrGain = new uint8_t[sizeof(vcsfw_frame_tag_t)+sizeof(vcsfw_frame_tag_reg16blk_t)+sizeof(uint32_t)];
			memcpy(&(arrGain[0]), &frame_tag_gain, sizeof(vcsfw_frame_tag_t));
			memcpy(&(arrGain[sizeof(vcsfw_frame_tag_t)]), &frame_tag_reg16blk_gain, sizeof(vcsfw_frame_tag_reg16blk_t));
			memcpy(&(arrGain[sizeof(vcsfw_frame_tag_t)+sizeof(vcsfw_frame_tag_reg16blk_t)]), &gainValue, sizeof(uint32_t));
			rc = _pSynModule->FpIotawrite(arrGain, sizeof(vcsfw_frame_tag_t)+sizeof(vcsfw_frame_tag_reg16blk_t)+sizeof(uint32_t), VCSFW_IOTA_ITYPE_FRAME_BASE);
			delete[] arrGain;
			arrGain = NULL;
			if (0 != rc)
			{
				Exception.SetError(rc);
				Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(gain) is failed!");
				throw Exception;
				return;
			}
			
			//REG32BLK	WOF DCA OFFSETS
			vcsfw_frame_tag_t frame_tag_offset;
			frame_tag_offset.tagid = VCSFW_FRAME_TAG_REG32BLK;
			frame_tag_offset.nwords = 2;
			frame_tag_offset.flags = 0x20;

			vcsfw_frame_tag_reg32blk_t frame_tag_reg32blk_gain;
			frame_tag_reg32blk_gain.regbase = offsetRegister;
			frame_tag_reg32blk_gain.nregs = 1;

			uint32_t offsetValue32bits = 0;
			offsetValue32bits = (offsetValue << 12) + offsetValue;

			uint8_t *arrOffset = new uint8_t[sizeof(vcsfw_frame_tag_t)+sizeof(vcsfw_frame_tag_reg32blk_t)+sizeof(uint32_t)];
			memcpy(&(arrOffset[0]), &frame_tag_offset, sizeof(vcsfw_frame_tag_t));
			memcpy(&(arrOffset[sizeof(vcsfw_frame_tag_t)]), &frame_tag_reg32blk_gain, sizeof(vcsfw_frame_tag_reg32blk_t));
			memcpy(&(arrOffset[sizeof(vcsfw_frame_tag_t)+sizeof(vcsfw_frame_tag_reg32blk_t)]), &offsetValue32bits, sizeof(uint32_t));
			rc = _pSynModule->FpIotawrite(arrOffset, sizeof(vcsfw_frame_tag_t)+sizeof(vcsfw_frame_tag_reg32blk_t)+sizeof(uint32_t), VCSFW_IOTA_ITYPE_FRAME_BASE);
			delete[] arrOffset;
			arrOffset = NULL;
			if (0 != rc)
			{
				Exception.SetError(rc);
				Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(offset) is failed!");
				throw Exception;
				return;
			}

			////CONFIG_WOF_THRESHOLDS
			//vcsfw_config_wof_thresholds_t config_wof_thresholds;
			//config_wof_thresholds.signal = signalValue;
			//config_wof_thresholds.hyst = 5;
			//config_wof_thresholds.reduction = 12;
			//rc = _pSynModule->FpIotawrite((uint8_t*)&config_wof_thresholds, sizeof(vcsfw_config_wof_thresholds_t), VCSFW_IOTA_ITYPE_CONFIG_WOF_THRESHOLDS);
			//if (0 != rc)
			//{
			//	Exception.SetError(rc);
			//	Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(CONFIG_WOF_THRESHOLDS) is failed!");
			//	throw Exception;
			//	return;
			//}

			//REG32BLK
			/*vcsfw_frame_tag_t frame_tag_ctrl;
			frame_tag_ctrl.tagid = VCSFW_FRAME_TAG_REG32BLK;
			frame_tag_ctrl.nwords = 2;
			frame_tag_ctrl.flags = 0x20;

			vcsfw_frame_tag_reg32blk_t frame_tag_reg32blk_ctrl;
			frame_tag_reg32blk_ctrl.regbase = ctrlRegister;
			frame_tag_reg32blk_ctrl.nregs = 1;

			uint32_t ctrlValue32bits = 0;
			uint32_t FD_ACCUM = 0, FU_ACCUM = 0, FD_SAMPLES = 1, FU_SAMPLES = 1, FH_SAMPLES = 1;
			ctrlValue32bits = (FD_SAMPLES << 2) + (FU_SAMPLES << 12) + (FH_SAMPLES << 22);

			uint8_t *arrCtrl = new uint8_t[sizeof(vcsfw_frame_tag_t)+sizeof(vcsfw_frame_tag_reg32blk_t)+sizeof(uint32_t)];
			memcpy(&(arrCtrl[0]), &frame_tag_offset, sizeof(vcsfw_frame_tag_t));
			memcpy(&(arrCtrl[sizeof(vcsfw_frame_tag_t)]), &frame_tag_reg32blk_ctrl, sizeof(vcsfw_frame_tag_reg32blk_t));
			memcpy(&(arrCtrl[sizeof(vcsfw_frame_tag_t)+sizeof(vcsfw_frame_tag_reg32blk_t)]), &ctrlValue32bits, sizeof(uint32_t));
			rc = _pSynModule->FpIotawrite(arrCtrl, sizeof(vcsfw_frame_tag_t)+sizeof(vcsfw_frame_tag_reg32blk_t)+sizeof(uint32_t), VCSFW_IOTA_ITYPE_FRAME_BASE);
			delete[] arrCtrl;
			arrCtrl = NULL;
			if (0 != rc)
			{
				Exception.SetError(rc);
				Exception.SetDescription("BravoProgrammingIOTA::FpIotawrite(ctrl) is failed!");
				throw Exception;
				return;
			}*/
		}
	}

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