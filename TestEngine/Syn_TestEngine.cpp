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

	CalibrateTestData *calibrationTestData = NULL;
	AcqImageNoFingerTestData *acqImageNoFingerTestData = NULL;
	AcqImageFingerTestData *acqImageFingerTestData = NULL;

	//Teststep info
	for (size_t t = 0; t < _pSynDutUtils->_pDutTestResult->list_testdata.size(); t++)
	{
		SynTestData *Test_data = _pSynDutUtils->_pDutTestResult->list_testdata[t];
		if (NULL == Test_data)
			continue;

		string strTestStepName = Test_data->data_name;
		if ("InitializationStep" == strTestStepName)
		{
			InitializationTestData *InitTestData = static_cast<InitializationTestData*>(Test_data);
			if (NULL != InitTestData)
				fprintf(pFile, "\nInitialization,%s,%d ms\n", InitTestData->pass ? "Pass" : "Fail", InitTestData->test_time);
		}
		else if ("ProgrammingMissionFirmware" == strTestStepName)
		{
			ProgrammingMFTestData *ProgrammingMissionFWTestData = static_cast<ProgrammingMFTestData*>(Test_data);
			if (NULL != ProgrammingMissionFWTestData)
			{
				fprintf(pFile, "\nProgrammingMissionFirmware,%s,%.0f ms,Firmware buildnumber,last MissionFirmware build number\n", ProgrammingMissionFWTestData->pass ? "Pass" : "Fail", ProgrammingMissionFWTestData->test_time);
				fprintf(pFile, ",,,%d,%d\n", ProgrammingMissionFWTestData->FirmwareBuildNumber,ProgrammingMissionFWTestData->ExistsMissionFirmwareNumber);
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
		else if ("Calibrate" == strTestStepName)
		{
			calibrationTestData = static_cast<CalibrateTestData*>(Test_data);
			if (NULL != calibrationTestData)
			{
				fprintf(pFile, "\nCalibrate,%s,%.0f ms\n", calibrationTestData->pass ? "Pass" : "Fail", calibrationTestData->test_time);
			}
		}
		else if ("AcqImgNoFinger" == strTestStepName)
		{
			acqImageNoFingerTestData = static_cast<AcqImageNoFingerTestData*>(Test_data);
			/*if (NULL != acqImageNoFingerTestData)
			{
				fprintf(pFile, "\nCalibrate,%s,%.0f ms\n", calibrationTestData->pass ? "Pass" : "Fail", calibrationTestData->test_time);
			}*/
		}
		else if ("AcqImgFinger" == strTestStepName)
		{
			acqImageFingerTestData = static_cast<AcqImageFingerTestData*>(Test_data);
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
			FinalizationTestData *FinalTestData = static_cast<FinalizationTestData*>(Test_data);
			if (NULL != FinalTestData)
				fprintf(pFile, "\nFinalization,%s,%d ms\n", FinalTestData->pass ? "Pass" : "Fail", FinalTestData->test_time);
		}
		else
		{
			continue;
		}
	}

	//FW_BL && LNA_BL
	if (NULL != calibrationTestData)
	{
		int k = 0;
		fprintf(pFile, "\nFW_BL");
		for (int i = 0; i < RowNumber; i++)
		{
			fprintf(pFile, "\n");
			for (int j = 0; j < ColumnNumber; j++)
			{
				fprintf(pFile, "%d,", calibrationTestData->FWBaseline[k]);
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
				fprintf(pFile, "%d,", calibrationTestData->LNABaseline[k]);
				k++;
			}
		}
	}

	if (NULL != acqImageNoFingerTestData)
	{
		fprintf(pFile, "\n\nAverage No Finger,%s,%.0f ms", acqImageNoFingerTestData->pass ? "Pass" : "Fail", acqImageNoFingerTestData->test_time);

		int k = 0;
		for (int i = 0; i < RowNumber; i++)
		{
			fprintf(pFile, "\n");
			for (int j = 0; j < ColumnNumber; j++)
			{
				fprintf(pFile, "%d,", acqImageNoFingerTestData->arrImage[k]);
				k++;
			}
		}
	}

	if (NULL != acqImageFingerTestData)
	{
		fprintf(pFile, "\n\nNormalized Finger,%s,%.0f ms", acqImageFingerTestData->pass ? "Pass" : "Fail", acqImageFingerTestData->test_time);
		int k = 0;
		for (int i = 0; i < RowNumber; i++)
		{
			fprintf(pFile, "\n");
			for (int j = 0; j < ColumnNumber; j++)
			{
				fprintf(pFile, "%d,", acqImageFingerTestData->arrImage[k]);
				k++;
			}
		}
	}

	fclose(pFile);

	return 0;
}