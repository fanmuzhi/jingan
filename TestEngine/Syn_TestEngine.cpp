#include "Syn_TestEngine.h"
#include "Syn_Exception.h"

//std
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <regex>

//stat
#include <sys/stat.h>

Syn_TestEngine::Syn_TestEngine(uint32_t EngineNumber, string strDeviceSerNumber, string strConfigFilePath)
:_TestEngineNumber(EngineNumber)
, _strDeviceSerialNumber(strDeviceSerNumber)
, _strConfigFilePath(strConfigFilePath)
, _pSynDutUtils(NULL)
, _pSyn_Bridge(NULL)
, _pSyn_Module(NULL)
, _State(closed)
{
}

Syn_TestEngine::~Syn_TestEngine()
{
	if (NULL != _pSynDutUtils)
	{
		delete _pSynDutUtils;
		_pSynDutUtils = NULL;
	}

	if (NULL != _pSyn_Bridge)
	{
		delete _pSyn_Bridge;
		_pSyn_Bridge = NULL;
	}

	if (NULL != _pSyn_Module)
	{
		delete _pSyn_Module;
		_pSyn_Module = NULL;
	}
}

uint32_t Syn_TestEngine::CreateTestEngine(uint32_t TestEngineNumber, string strDeviceSerialNumber, string strConfigFilePath, Syn_TestEngine * &opTestEngine)
{
	opTestEngine = NULL;

	uint32_t rc = 0;

	opTestEngine = new Syn_TestEngine(TestEngineNumber, strDeviceSerialNumber, strConfigFilePath);
	rc = opTestEngine->Init();
	if (0 != rc)
	{
		delete opTestEngine;
		opTestEngine = NULL;
	}

	return rc;

}

uint32_t Syn_TestEngine::Init()
{
	_State = error;

	//xml config file parse
	Syn_Config_Utils_MT *pConfigUtilsMT = NULL;
	uint32_t rc = Syn_Config_Utils_MT::CreateConfigUtils_MT(_strConfigFilePath, pConfigUtilsMT);
	if (0!=rc || NULL == pConfigUtilsMT)
	{
		return rc;
	}

	rc = pConfigUtilsMT->GetConfigMTInfo(_Config_MT_Info);
	delete pConfigUtilsMT;
	pConfigUtilsMT = NULL;
	if (0 != rc)
	{
		return rc;
	}

	string strDutController(_Config_MT_Info.strDutController);
	devicetype DeviceType;
	uint32_t clockrate = M5_CLOCKRATE;
	if (string("MPC04") == strDutController)
	{
		DeviceType = spi_mpc04;
		clockrate = MPC04_CLOCKRATE;
	}
	else
	{
		DeviceType = spi_m5;
		clockrate = M5_CLOCKRATE;
	}

	//Create Bridge
	rc = syn_bridge::CreateDeviceInstance(_strDeviceSerialNumber, DeviceType, _pSyn_Bridge);
	if (0 != rc)
	{
		return rc;
	}

	//Module
	string strModuleType = _Config_MT_Info.strDutType;
	FpBravoModule::BravoSensorType SensorType;
	if (string("Denali") == strModuleType)
	{
		SensorType = FpBravoModule::Denali;
	}
	else if (string("Hayes") == strModuleType)
	{
		SensorType = FpBravoModule::Hayes;
	}
	else if (string("Shasta") == strModuleType)
	{
		SensorType = FpBravoModule::Shasta;
	}
	else
	{
		SensorType = FpBravoModule::Denali;
	}
	rc = FpBravoModule::CreateModuleInstance(SensorType, _pSyn_Bridge, _pSyn_Module);
	if (0 != rc)
	{
		delete _pSyn_Bridge; 
		_pSyn_Bridge = NULL;
		return rc;
	}

	rc = _pSyn_Bridge->SetPortSPI(clockrate);

	_State = closed;

	return rc;
}

uint32_t Syn_TestEngine::Open()
{
	uint32_t rc = 0;

	if (_State == running)
	{
		return ERROR_TESTENGINE_STATE;
	}

	//Create DutUtils
	if (NULL != _pSynDutUtils)
	{
		delete _pSynDutUtils;
		_pSynDutUtils = NULL;
	}
	_pSynDutUtils = new Syn_Dut_Utils();
	_pSynDutUtils->Config_MT_Info = _Config_MT_Info;

	//fill info

	size_t stringSize = _strConfigFilePath.size();
	size_t lastBackslashIndex = _strConfigFilePath.find_last_of("/");
	if (std::string::npos == lastBackslashIndex)
	{
		lastBackslashIndex = _strConfigFilePath.find_last_of("\\");
	}
	//_pSyn_Dut->_sConfigFileName = _strConfigFilePath.substr(lastBackslashIndex + 1, stringSize - lastBackslashIndex - 1);

	_State = idle;

	//_pSyn_Dut->_DeviceSerialNumber = _strSerialNumber;
	//_pSyn_Dut->_iSiteNumber = _iSiteNumber;

	return 0;
}

uint32_t Syn_TestEngine::GetTestData(dut_test_result * &opTestData)
{
	opTestData = NULL;

	if (_State == error || _State != data_ready)
	{
		return ERROR_TESTENGINE_STATE;
	}

	if (NULL != _pSynDutUtils)
	{
		opTestData = _pSynDutUtils->_pDutTestResult;

		_State = idle;
		return 0;
	}

	_State = error;

	return ERROR_TESTENGINE_DATA;
}

uint32_t Syn_TestEngine::Close()
{
	if (_State == running)
	{
		return ERROR_TESTENGINE_STATE;
	}

	if (NULL != _pSynDutUtils)
	{
		delete _pSynDutUtils;
		_pSynDutUtils = NULL;
	}

	_State = closed;

	return 0;
}

void Syn_TestEngine::GetTestStepList(vector<string> &oListTeststepName)
{
	oListTeststepName.clear();

	for (size_t i = 1; i <= _Config_MT_Info.list_TestStep_Info.size(); i++)
	{
		oListTeststepName.push_back(_Config_MT_Info.list_TestStep_Info[i - 1].strTestStepName);
	}
}

uint32_t Syn_TestEngine::ExecuteTestStep(string TestStepName, ExcuteType Type)
{
	uint32_t rc = 0;

	if (_State == error)
	{
		return ERROR_TESTENGINE_STATE;
	}
	if (_State != idle && _State != data_ready)
	{
		return ERROR_TESTENGINE_STATE;
	}

	string strArgsValue("");
	_Config_MT_Info.GetTestStepInfo(TestStepName, strArgsValue);
	Syn_TestStep *pTestStep = NULL;
	rc = Syn_TestStepFactory::CreateBravoTestStep(TestStepName, strArgsValue, _pSyn_Module, _pSynDutUtils, pTestStep);
	if (0 != rc || NULL == pTestStep)
	{
		_State = error;
		return rc;
	}

	_State = running;
	try
	{
		switch (Type)
		{
			case All:
				pTestStep->SetUp();
				pTestStep->Execute();
				pTestStep->ProcessData();
				pTestStep->CleanUp();
				break;

			case Setup:
				pTestStep->SetUp();
				break;

			case Excute:
				pTestStep->Execute();
				break;

			case ProcessData:
				pTestStep->ProcessData();
				break;

			case Cleanup:
				pTestStep->CleanUp();
				break;

			default:
				pTestStep->SetUp();
				pTestStep->Execute();
				pTestStep->ProcessData();
				pTestStep->CleanUp();
				break;
		}

		delete pTestStep;
		pTestStep = NULL;

		_State = data_ready;
	}
	catch (Syn_Exception ex)
	{
		try
		{
			_pSyn_Bridge->SetVoltages(0, 0);
		}
		catch (...){}
		delete pTestStep;
		pTestStep = NULL;

		_State = error;
		//_strErrorMessage = ex.GetDescription();
		//_uiErrorFlag = ex.GetError();
		return ex.GetError();
	}

	return 0;
}

uint32_t Syn_TestEngine::WriteLog(string strFolderPath, string strFileName)
{
	if (-1 == _access(strFolderPath.c_str(), 2))
	{
		return ERROR_ENGINELOG_FOLDER;
	}
	if (NULL == _pSynDutUtils)
	{
		return ERROR_DUTUTILS_NULL;
	}
	if (NULL == _pSynDutUtils->_pDutTestResult)
	{
		return ERROR_TESTENGINE_DATA;
	}

	int RowNumber = _Config_MT_Info.rowNumber;
	int ColumnNumber = _Config_MT_Info.columnNumber;

	string strSensorSerialNumber = _pSynDutUtils->_pDutTestResult->strSensorSerialNumber;

	string strFilePath("");
	if (0 == strFileName.size())
	{
		strFilePath = strFolderPath + string("/") + strSensorSerialNumber + string(".csv");
		int iCount(1);
		struct stat buffer;
		bool fileExists = stat(strFilePath.c_str(), &buffer) == 0 ? true : false;
		while (fileExists)
		{
			strFilePath = strFolderPath + string("/") + strSensorSerialNumber + "_" + to_string(iCount) + string(".csv");
			iCount++;
			fileExists = stat(strFilePath.c_str(), &buffer) == 0 ? true : false;
		}
	}
	else
	{
		if (string::npos != strFileName.find_first_of(".csv"))
		{
			strFilePath = strFolderPath + "\\" + strFileName;
		}
		else
		{
			strFilePath = strFolderPath + "\\" + strFileName + ".csv";
		}
	}

	FILE *pFile = fopen(strFilePath.c_str(), "a");
	if (NULL == pFile)
	{
		return ERROR_ENGINELOG_FILE;
	}

	//Basic info
	fprintf(pFile, "%%%%%%%%%%%%%%%%%%%%%%\n");
	fprintf(pFile, "MTLog,%d,%s\n", _TestEngineNumber, _strDeviceSerialNumber.c_str());

	fprintf(pFile, "Version,%s\n", SW_VERSION);//Version

	//Config file path
	fprintf(pFile, "Config file,%s\n", _strConfigFilePath.c_str());

	fprintf(pFile, "%%%%%%%%%%%%%%%%%%%%%%\n\n");

	time_t rawtime;
	struct tm * timeinfo;
	wchar_t buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	wcsftime(buffer, 80, L"%d-%b-%Y %I:%M:%S %p %Z", timeinfo);
	fprintf(pFile, "Test time,%ls\n\n", buffer);

	//Sensor Serial Number
	fprintf(pFile, "Sensor Serial Number,%s\n", strSensorSerialNumber.c_str());

	CalibrateTestData *pCalibrationTestData = NULL;
	AcqImageNoFingerTestData *pAcqImageNoFingerTestData = NULL;
	AcqImageFingerTestData *pAcqImageFingerTestData = NULL;
	WOF_BaselineTestData *pWOF_BaselineTestData = NULL;
	WOF_SignalTestData *pWOF_SignalTestData = NULL;
	FinalizationTestData *pFinalTestData = NULL;

	//Teststep info
	for (size_t t = 0; t < _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
	{
		SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t];
		if (NULL == Test_data)
			continue;

		string strTestStepName = Test_data->data_name;
		if ("InitializationStep" == strTestStepName)
		{
			InitializationTestData *pInitTestData = static_cast<InitializationTestData*>(Test_data);
			if (NULL != pInitTestData)
				fprintf(pFile, "\nInitialization,%s,%.f ms\n", pInitTestData->pass ? "Pass" : "Fail", pInitTestData->test_time);
		}
		else if ("ProgrammingMissionFirmware" == strTestStepName)
		{
			ProgrammingMFTestData *pProgrammingMFTestData = static_cast<ProgrammingMFTestData*>(Test_data);
			if (NULL != pProgrammingMFTestData)
			{
				fprintf(pFile, "\nProgrammingMissionFirmware,%s,%.0f ms,Firmware buildnumber,last MissionFirmware build number\n", pProgrammingMFTestData->pass ? "Pass" : "Fail", pProgrammingMFTestData->test_time);
				fprintf(pFile, ",,,%d,%d\n", pProgrammingMFTestData->FirmwareBuildNumber, pProgrammingMFTestData->ExistsMissionFirmwareNumber);
			}
		}
		else if ("ProgrammingIOTA_BIN" == strTestStepName)
		{
			ProgrammingIOTATestData *ProgrammingIOTABINTestData = static_cast<ProgrammingIOTATestData*>(Test_data);
			if (NULL != ProgrammingIOTABINTestData)
			{
				fprintf(pFile, "\nProgrammingIOTA_BIN,%s,%.0f ms\n", ProgrammingIOTABINTestData->pass ? "Pass" : "Fail", ProgrammingIOTABINTestData->test_time);
			}
		}
		else if ("DRdyTest" == strTestStepName)
		{
			DrdyTestData *pDrdyTestData = static_cast<DrdyTestData*>(Test_data);
			if (NULL != pDrdyTestData)
			{
				fprintf(pFile, "\nDRdyTest,%s,%.0f ms\n", pDrdyTestData->pass ? "Pass" : "Fail", pDrdyTestData->test_time);
			}
		}
		else if ("SleepCurrentTest" == strTestStepName)
		{
			SleepCurrentTestData *pSleepCurrentTestData = static_cast<SleepCurrentTestData*>(Test_data);
			if (NULL != pSleepCurrentTestData)
			{
				fprintf(pFile, "\nSleepCurrentTest,%s,%.0f ms,spivcc current (uA),vcc current (uA)\n", pSleepCurrentTestData->pass ? "Pass" : "Fail", pSleepCurrentTestData->test_time);
				fprintf(pFile, ",,,%f,%f\n", (float)(pSleepCurrentTestData->spivcc_current_uA), (float)(pSleepCurrentTestData->vcc_current_uA));
			}
		}
		else if ("Calibrate" == strTestStepName)
		{
			pCalibrationTestData = static_cast<CalibrateTestData*>(Test_data);
			if (NULL != pCalibrationTestData)
			{
				fprintf(pFile, "\nCalibrate,%s,%.0f ms\n", pCalibrationTestData->pass ? "Pass" : "Fail", pCalibrationTestData->test_time);
			}
		}
		else if ("AcqImgNoFinger" == strTestStepName)
		{
			pAcqImageNoFingerTestData = static_cast<AcqImageNoFingerTestData*>(Test_data);
			/*if (NULL != acqImageNoFingerTestData)
			{
				fprintf(pFile, "\nCalibrate,%s,%.0f ms\n", calibrationTestData->pass ? "Pass" : "Fail", calibrationTestData->test_time);
			}*/
		}
		else if ("AcqImgFinger" == strTestStepName)
		{
			pAcqImageFingerTestData = static_cast<AcqImageFingerTestData*>(Test_data);
		}
		else if ("WOF_Baseline" == strTestStepName)
		{
			pWOF_BaselineTestData = static_cast<WOF_BaselineTestData*>(Test_data);
		}
		else if ("WOF_Signal" == strTestStepName)
		{
			pWOF_SignalTestData = static_cast<WOF_SignalTestData*>(Test_data);
		}
		else if ("CurrentTest" == strTestStepName)
		{
			CurrentTestData *pCurrentTestData = static_cast<CurrentTestData*>(Test_data);
			if (NULL != pCurrentTestData)
			{
				fprintf(pFile, "\nCurrent Test,%s,%.0f ms,Digital image acq current (uA),Analog image acq current (uA)\n", pCurrentTestData->pass ? "Pass" : "Fail", pCurrentTestData->test_time);
				fprintf(pFile, ",,,%f,%f\n", (float)(pCurrentTestData->ImageAcqDigCurrent_uA), (float)(pCurrentTestData->ImageAcqAnaCurrent_uA));
			}
		}
		else if ("SNRTest" == strTestStepName)
		{
			SNRTestData *pSNRTestData = static_cast<SNRTestData*>(Test_data);
			if (NULL != pSNRTestData)
			{
				fprintf(pFile, "\nSNRTest(simply),%s,%.0f ms,SNR,Signal,Noise\n", pSNRTestData->pass ? "Pass" : "Fail", pSNRTestData->test_time);
				fprintf(pFile, ",,,%f,%d,%f\n", pSNRTestData->snrValue, pSNRTestData->signalValue, pSNRTestData->noiseValue);
			}
		}
		else if ("SharpnessTest" == strTestStepName)
		{
			SharpnessData *pSharpnessData = static_cast<SharpnessData*>(Test_data);
			if (NULL != pSharpnessData)
			{
				fprintf(pFile, "\nSharpness Test,%s,%.0f ms,", pSharpnessData->pass ? "Pass" : "Fail", pSharpnessData->test_time);
				fprintf(pFile, "Variation(%%),Zone1,Zone2,Zone3,Overall\n");
				fprintf(pFile, ",,,%f,%d,%d,%d,%d", pSharpnessData->percent, (int)pSharpnessData->SHARPNESS[0], (int)pSharpnessData->SHARPNESS[1], (int)pSharpnessData->SHARPNESS[2], (int)pSharpnessData->SHARPNESS[3]);
				fprintf(pFile, "\n");
			}
		}
		else if ("Imperfections" == strTestStepName)
		{
			ImperfectionsTestData *pImperfectionsTestData = static_cast<ImperfectionsTestData*>(Test_data);
			if (NULL != pImperfectionsTestData)
			{
				fprintf(pFile, "\nImperfections,%s,%.0f ms,Zone1,Zone2,Zone3,Zone4,Zone5,Zone6,Zone_OverAll\n", pImperfectionsTestData->pass ? "Pass" : "Fail", pImperfectionsTestData->test_time);
				fprintf(pFile, ",,,%d,%d,%d,%d,%d,%d,%d\n", pImperfectionsTestData->bubble_check_data[1].nBubbleMeasure_x10, 
					pImperfectionsTestData->bubble_check_data[2].nBubbleMeasure_x10, pImperfectionsTestData->bubble_check_data[3].nBubbleMeasure_x10,
					pImperfectionsTestData->bubble_check_data[4].nBubbleMeasure_x10, pImperfectionsTestData->bubble_check_data[5].nBubbleMeasure_x10,
					pImperfectionsTestData->bubble_check_data[6].nBubbleMeasure_x10, pImperfectionsTestData->bubble_check_data[0].nBubbleMeasure_x10);
			}
		}
		else if ("ProgrammingIOTA_DATA" == strTestStepName)
		{
			ProgrammingIOTATestData *ProgrammingIOTADATATestData = static_cast<ProgrammingIOTATestData*>(Test_data);
			if (NULL != ProgrammingIOTADATATestData)
			{
				fprintf(pFile, "\nProgrammingIOTA_DATA,%s,%.0f ms\n", ProgrammingIOTADATATestData->pass ? "Pass" : "Fail", ProgrammingIOTADATATestData->test_time);
			}
		}
		else if ("FinalizationStep" == strTestStepName)
		{
			pFinalTestData = static_cast<FinalizationTestData*>(Test_data);
		}
		else
		{
			continue;
		}
	}

	//WOF
	if (NULL != pWOF_BaselineTestData || NULL != pWOF_SignalTestData)
	{
		fprintf(pFile, "\nWOF,%s,%.0f ms,", pWOF_SignalTestData->pass ? "Pass" : "Fail", pWOF_BaselineTestData->test_time+pWOF_SignalTestData->test_time);
		fprintf(pFile, "SelectedGain,SelectedOffset,SelectedSignal,Gain1,Offset1,Signal1,Gain2,Offset2,Signal2,Gain3,Offset3,Signal3,Gain4,Offset4,Signal4,Gain5,Offset5,Signal5\n");
		fprintf(pFile, ",,,");

		fprintf(pFile, "%d,%d,%d,", pWOF_SignalTestData->selectedGain, pWOF_SignalTestData->selectedOffset, pWOF_SignalTestData->selectedSingal);

		for (int i = 0; i<BRAVO_WOF_COUNTS_MAX; i++)
			fprintf(pFile, "%d,%d,%d,", pWOF_BaselineTestData->arrWOFBaseline[i], pWOF_BaselineTestData->arrWOFOffset[i], pWOF_SignalTestData->arrWOFSignal[i]);
		fprintf(pFile, "\n");
	}


	if (NULL != pFinalTestData)
		fprintf(pFile, "\nFinalization,%s,%.f ms\n", pFinalTestData->pass ? "Pass" : "Fail", pFinalTestData->test_time);

	//FW_BL && LNA_BL
	if (NULL != pCalibrationTestData)
	{
		int k = 0;
		fprintf(pFile, "\nFW_BL");
		for (int i = 0; i < RowNumber; i++)
		{
			fprintf(pFile, "\n");
			for (int j = 0; j < ColumnNumber; j++)
			{
				fprintf(pFile, "%d,", pCalibrationTestData->FWBaseline[k]);
				k++;
			}
		}

		fprintf(pFile, "\n\nLNA_BL");
		k = 0;
		for (int i = 0; i < RowNumber; i++)
		{
			fprintf(pFile, "\n");
			for (int j = 0; j < ColumnNumber; j++)
			{
				fprintf(pFile, "%d,", pCalibrationTestData->LNABaseline[k]);
				k++;
			}
		}
	}

	if (NULL != pAcqImageNoFingerTestData)
	{
		fprintf(pFile, "\n\nAverage No Finger,%s,%.0f ms", pAcqImageNoFingerTestData->pass ? "Pass" : "Fail", pAcqImageNoFingerTestData->test_time);

		int k = 0;
		for (int i = 0; i < RowNumber; i++)
		{
			fprintf(pFile, "\n");
			for (int j = 0; j < ColumnNumber; j++)
			{
				fprintf(pFile, "%d,", pAcqImageNoFingerTestData->arrImage[k]);
				k++;
			}
		}
	}

	if (NULL != pAcqImageFingerTestData)
	{
		fprintf(pFile, "\n\nNormalized Finger,%s,%.0f ms", pAcqImageFingerTestData->pass ? "Pass" : "Fail", pAcqImageFingerTestData->test_time);
		int k = 0;
		for (int i = 0; i < RowNumber; i++)
		{
			fprintf(pFile, "\n");
			for (int j = 0; j < ColumnNumber; j++)
			{
				fprintf(pFile, "%d,", pAcqImageFingerTestData->arrImage[k]);
				k++;
			}
		}
	}

	fclose(pFile);

	return 0;
}