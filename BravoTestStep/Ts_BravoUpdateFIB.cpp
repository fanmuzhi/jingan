#include "Ts_BravoUpdateFIB.h"

Ts_BravoUpdateFIB::Ts_BravoUpdateFIB(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pUpdateFIBTestData(NULL)
{
}

Ts_BravoUpdateFIB::~Ts_BravoUpdateFIB()
{
}

void Ts_BravoUpdateFIB::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoUpdateFIB::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoUpdateFIB::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

	_pUpdateFIBTestData = new UpdateFIBTestData();
	_pUpdateFIBTestData->data_name = _strName;
}

void Ts_BravoUpdateFIB::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	//superpeekpoke
	Syn_Patch_Info fibwritePatch;
	if (!_pSynDutUtils->Config_MT_Info.GetPatchInfo("fibwrite", fibwritePatch))
	{
		Exception.SetError(ERROR_CONFIG_DATA_LOST);
		Exception.SetDescription("BravoUpdateFIB::fibwritePatch not exists!");
		throw Exception;
		return;
	}
	if (NULL == fibwritePatch.patchbufp || 0 == fibwritePatch.patchbuflen)
	{
		Exception.SetError(ERROR_CONFIG_DATA_LOST);
		Exception.SetDescription("BravoUpdateFIB::IOTAWritePatch has no value!");
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
		Exception.SetDescription("BravoUpdateFIB::checkBLMode is failed!");
		throw Exception;
		return;
	}
	if (!InBootloaderMode)
	{
		Exception.SetError(ERROR_BOOTLOADERMODE_CHECK);
		Exception.SetDescription("BravoUpdateFIB::InBootloaderMode is false!");
		throw Exception;
		return;
	}
	rc = _pSynModule->FpLoadPatch(fibwritePatch.patchbufp, fibwritePatch.patchbuflen);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoUpdateFIB::FpLoadPatch is failed!");
		throw Exception;
		return;
	}

	_pUpdateFIBTestData->executed = true;

	/*
		# HWDEF_FIB_MISSIONSIZE
		$DACTYL poke 0x50820ef8 0x8000f000
		# HWDEF_FIB_IOTAOFF
		$DACTYL poke 0x50820f00 0x8000f000
		# HWDEF_FIB_IOTASIZE
		$DACTYL poke 0x50820f04 0x80008ff8
		$DACTYL peek 0x50820ef8 0x50820f00 0x50820f04
	*/

	rc = _pSynModule->FpPoke(0x50820ef8, 0x8000f000);
	if (0 != rc)
	{
		_pSynModule->FpExitBLMode();
		Exception.SetError(rc);
		Exception.SetDescription("BravoUpdateFIB::FpPoke is failed(1)!");
		throw Exception;
		return;
	}

	rc = _pSynModule->FpPoke(0x50820f00, 0x8000f000);
	if (0 != rc)
	{
		_pSynModule->FpExitBLMode();
		Exception.SetError(rc);
		Exception.SetDescription("BravoUpdateFIB::FpPoke is failed(2)!");
		throw Exception;
		return;
	}

	rc = _pSynModule->FpPoke(0x50820f04, 0x80008ff8);
	if (0 != rc)
	{
		_pSynModule->FpExitBLMode();
		Exception.SetError(rc);
		Exception.SetDescription("BravoUpdateFIB::FpPoke is failed(3)!");
		throw Exception;
		return;
	}

	uint32_t peekvalue1(0), peekvalue2(0), peekvalue3(0);
	rc = _pSynModule->FpPeek(0x50820ef8, 4, peekvalue1);
	if (0 != rc)
	{
		_pSynModule->FpExitBLMode();
		Exception.SetError(rc);
		Exception.SetDescription("BravoUpdateFIB::FpPeek is failed(1)!");
		throw Exception;
		return;
	}

	rc = _pSynModule->FpPeek(0x50820f00, 4, peekvalue2);
	if (0 != rc)
	{
		_pSynModule->FpExitBLMode();
		Exception.SetError(rc);
		Exception.SetDescription("BravoUpdateFIB::FpPeek is failed(2)!");
		throw Exception;
		return;
	}

	rc = _pSynModule->FpPeek(0x50820f04, 4, peekvalue3);
	if (0 != rc)
	{
		_pSynModule->FpExitBLMode();
		Exception.SetError(rc);
		Exception.SetDescription("BravoUpdateFIB::FpPeek is failed(3)!");
		throw Exception;
		return;
	}

	rc = _pSynModule->FpExitBLMode();
	rc = _pSynModule->FpTidleSet(0);

	_pUpdateFIBTestData->pass = true;

	if (0x8000f000 != peekvalue1 || 0x8000f000 != peekvalue2 || 0x80008ff8 != peekvalue3)
		_pUpdateFIBTestData->pass = false;
}

void Ts_BravoUpdateFIB::ProcessData()
{
	if (_pUpdateFIBTestData->pass)
		_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type("UpdateFIB", "Pass"));
	else
	{
		_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type("UpdateFIB", "Fail"));
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("154");
	}
}

void Ts_BravoUpdateFIB::CleanUp()
{
	CalculateTestTime(_pUpdateFIBTestData->test_time);

	StoreTestData(_pUpdateFIBTestData->data_name, static_cast<SynTestData*>(_pUpdateFIBTestData));
}
