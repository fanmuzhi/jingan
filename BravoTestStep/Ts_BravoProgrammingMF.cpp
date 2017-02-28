#include "Ts_BravoProgrammingMF.h"

Ts_BravoProgrammingMF::Ts_BravoProgrammingMF(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pProgrammingMFTestData(NULL)
{
}

Ts_BravoProgrammingMF::~Ts_BravoProgrammingMF()
{
}

void Ts_BravoProgrammingMF::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoProgrammingMF::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoProgrammingMF::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	//vector<string> listOfArgValue;
	//ParseTestStepArgs(strTestArgs, listOfArgValue);
	//size_t iListSize = listOfArgValue.size();

	_pProgrammingMFTestData = new ProgrammingMFTestData();
	_pProgrammingMFTestData->data_name = _strName;
}

void Ts_BravoProgrammingMF::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	_pProgrammingMFTestData->FirmwareBuildNumber = 0;
	_pProgrammingMFTestData->ExistsMissionFirmwareNumber = 0;

	vcsfw_reply_get_version_t get_version;
	
	_pSynModule->FpExitBLMode();
	bool InBLMode(false);
	bool Flasherase(false);
	_pSynModule->checkBLMode(InBLMode);
	if (!InBLMode)
	{
		rc = _pSynModule->FpGetVersion((uint8_t*)&get_version, sizeof(vcsfw_reply_get_version_t));
		if (0 != rc)
		{
			Exception.SetError(rc);
			Exception.SetDescription("BravoProgrammingMF::FpGetVersion is failed!");
			throw Exception;
			return;
		}
		_pProgrammingMFTestData->ExistsMissionFirmwareNumber = get_version.buildnum;

		Flasherase = true;
	}

	_pSynModule->FpEnterBLMode();
	_pSynModule->checkBLMode(InBLMode);
	if (!InBLMode)
	{
		Exception.SetError(ERROR_BOOTLOADERMODE_CHECK);
		Exception.SetDescription("BravoProgrammingMF::EnterBLMode is failed!");
		throw Exception;
		return;
	}
	rc = _pSynModule->FpGetVersion((uint8_t*)&get_version, sizeof(vcsfw_reply_get_version_t));
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoProgrammingMF::FpGetVersion2 is failed!");
		throw Exception;
		return;
	}
	_pProgrammingMFTestData->FirmwareBuildNumber = get_version.buildnum;

	//erase
	if (Flasherase)
	{
		Syn_Patch_Info FlashErasePatch;
		_pSynDutUtils->Config_MT_Info.GetPatchInfo("flasherase", FlashErasePatch);
		if (NULL == FlashErasePatch.patchbufp || 0 == FlashErasePatch.patchbuflen)
		{
			Exception.SetError(ERROR_CONFIG_DATA_LOST);
			Exception.SetDescription("BravoProgrammingMF::flasherase is NULL!");
			throw Exception;
			return;
		}
		rc = _pSynModule->FpLoadPatch(FlashErasePatch.patchbufp, FlashErasePatch.patchbuflen);
		if (0 != rc)
		{
			Exception.SetError(rc);
			Exception.SetDescription("BravoProgrammingMF::FpLoadPatch is failed!");
			throw Exception;
			return;
		}
		rc = _pSynModule->FpFlashErase(FlashEraseType::MISSION_FW);
		if (0 != rc)
		{
			Exception.SetError(rc);
			Exception.SetDescription("BravoProgrammingMF::FpFlashErase is failed!");
			throw Exception;
			return;
		}
		_pSynModule->FpReset();
	}

	/*
		denali : 0 and 1;
		hayes : 0, 1, 2 and 3
		shasta : 0 or 0 and 1
		so only check missionfw0 data
	*/
	Syn_Patch_Info MissionFirmwarePatch0, MissionFirmwarePatch1, MissionFirmwarePatch2, MissionFirmwarePatch3;
	_pSynDutUtils->Config_MT_Info.GetPatchInfo("missionfw0", MissionFirmwarePatch0);
	_pSynDutUtils->Config_MT_Info.GetPatchInfo("missionfw1", MissionFirmwarePatch1);
	_pSynDutUtils->Config_MT_Info.GetPatchInfo("missionfw2", MissionFirmwarePatch2);
	_pSynDutUtils->Config_MT_Info.GetPatchInfo("missionfw3", MissionFirmwarePatch3);
	if (NULL == MissionFirmwarePatch0.patchbufp || 0 == MissionFirmwarePatch0.patchbuflen)
	{
		Exception.SetError(ERROR_CONFIG_DATA_LOST);
		Exception.SetDescription("BravoProgrammingMF::missionfw0 is NULL!");
		throw Exception;
		return;
	}

	vector<Syn_Patch_Info> ListOfMFPatch;
	ListOfMFPatch.push_back(MissionFirmwarePatch0);
	ListOfMFPatch.push_back(MissionFirmwarePatch1);
	ListOfMFPatch.push_back(MissionFirmwarePatch2);
	ListOfMFPatch.push_back(MissionFirmwarePatch3);
	
	//load
	for (size_t t = 0; t < ListOfMFPatch.size(); t++)
	{
		if (NULL == ListOfMFPatch[t].patchbufp || 0 == ListOfMFPatch[t].patchbuflen)
			continue;

		rc = _pSynModule->FpLoadPatch(ListOfMFPatch[t].patchbufp, ListOfMFPatch[t].patchbuflen);
		if (0 != rc)
		{
			Exception.SetError(rc);
			Exception.SetDescription("BravoProgrammingMF::FpLoadPatch(MF) is failed!");
			throw Exception;
			return;
		}
	}

	_pSynModule->FpExitBLMode();

	rc = _pSynModule->FpTidleSet(0);

	_pProgrammingMFTestData->executed = true;
	_pProgrammingMFTestData->pass = true;
}

void Ts_BravoProgrammingMF::ProcessData()
{
	if (_pProgrammingMFTestData->pass)
		_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type("ProgrammingMissionFirmware", "Pass"));
	else
		_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type("ProgrammingMissionFirmware", "Fail"));
}

void Ts_BravoProgrammingMF::CleanUp()
{
	CalculateTestTime(_pProgrammingMFTestData->test_time);

	StoreTestData(_pProgrammingMFTestData->data_name, static_cast<SynTestData*>(_pProgrammingMFTestData));
}