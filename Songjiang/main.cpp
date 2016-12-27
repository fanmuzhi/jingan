
#include "windows.h"

#include <iostream>
#include <string>

//local
#include "Synaptics_DeviceManage.h"
#include "Synaptics_Site.h"
#include "Synaptics_Utils.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
int main(int argc, char *argv[])
{
	//MessageBox(NULL, std::to_string(argc).c_str(), TEXT("Message"), 0);

	std::string sConfigFilePath("");
	std::string sLogFilePath("");
	std::string sLogFileName("");
	if (3 == argc)
	{
		sConfigFilePath = argv[1];
		sLogFilePath = argv[2];
	}
	else if (4 <= argc)
	{
		sConfigFilePath = argv[1];
		sLogFilePath = argv[2];
		sLogFileName = argv[3];
	}
	else
	{
		MessageBox(NULL, "Wrong argument counts", TEXT("Message"), 0);
		return 1;
	}

	std::string deviceSN;
	SynapticsAdcBaseLineInfo abi;
	abi.m_nVdd = 1800;
	abi.m_nVio = 1800;
	abi.m_nVled = 3300;
	abi.m_nVddh = 3300;
	
	uint32_t rc(0);

	Synaptics_DeviceManage *pDeviceManage = new Synaptics_DeviceManage();
	Syn_DeviceType Type;
	rc = pDeviceManage->Open(M5);
	if (0 != rc)
	{
		rc = pDeviceManage->Open(MPC04);
		if (0 != rc)
		{
			MessageBox(NULL, "No device found%x", TEXT("Message"), rc);
			return rc;
		}
		Type = MPC04;
	}
	Type = M5;

	std::vector<std::string> listOfDeviceNumber;
	rc = pDeviceManage->GetSerialNumberList(listOfDeviceNumber);
	if (0 != listOfDeviceNumber.size())
	{
		for (size_t i = 0; i < listOfDeviceNumber.size();i++)
		{
			//Console.WriteLine(sn.ToString());
			pDeviceManage->UpdateADCOffsets(Type, listOfDeviceNumber[i], abi.m_nVdd, abi.m_nVio, abi.m_nVled, abi.m_nVddh, abi.arrAdcBaseLines);
		}
		deviceSN = listOfDeviceNumber[0];
	}
	else
	{
		delete pDeviceManage;
		pDeviceManage = NULL;
		MessageBox(NULL, "No device found%x", TEXT("Message"), rc);
		return rc;
	}

	delete pDeviceManage;
	pDeviceManage = NULL;

	std::cout << "DLL version:" << Synaptics_DLLVersion::GetDLLVersion() << std::endl;

	MessageBox(NULL, "Please remove the sensor...", TEXT("Message"), 0);

	std::string cfgPath = sConfigFilePath;//"D:\\ConfigFile(xml)\\Manhattan\\(580-006033-01r01)_OFilm_Manhattan_Huangpu_HuaweiSNR.xml";

	std::string strAdcBaseline;
	strAdcBaseline = std::to_string(abi.arrAdcBaseLines[0]) + "," + std::to_string(abi.arrAdcBaseLines[1]) + "," + std::to_string(abi.arrAdcBaseLines[2]) + "," + std::to_string(abi.arrAdcBaseLines[3]);
	std::cout << "ADC Baseline: " << strAdcBaseline << std::endl;

	MessageBox(NULL, "Please mount the sensor...", TEXT("Message"), 0);
	Synaptics_Site *site = NULL;
	try
	{
		site = new Synaptics_Site(1, deviceSN, cfgPath, abi);
	}
	catch(...)
	{
		return 2;
	}

	rc = site->Open();

	std::vector<std::string> listOfTestStep;
	listOfTestStep = site->GetTestStepList();
	if (0 == listOfTestStep.size())
	{
		site->Close();
		delete site;
		site = NULL;
		return 3;
	}

	Synaptics_TestResult *pTestResult = NULL;
	for (size_t step = 0; step < listOfTestStep.size();step++)
	{
		//display test step name
		//Console.WriteLine(step);

		//if waiting for fake finger
		if (listOfTestStep[step] == "WaitStimulus")
		{
			MessageBox(NULL, TEXT("please put stimulus on"), TEXT("Title"), 0);
		}
		else
		{

			ULONGLONG starttime = GetTickCount64();
			//execute test step
			rc = site->ExecuteTestStep(listOfTestStep[step]);
			if (rc != 0)
			{
				break;
			}

			//get test result
			rc = site->GetTestResult(pTestResult);
			if (rc != 0 || NULL == pTestResult)
			{
				break;
			}

			ULONGLONG finishtime = GetTickCount64();
			double RunningTime = (double)(finishtime - starttime);

			//
			std::string strTestStepName(listOfTestStep[step]);
			std::string strTestStepResult("");
			strTestStepResult = pTestResult->mapStepResult[strTestStepName];
			std::cout << strTestStepName << ":" << strTestStepResult << ","<< RunningTime <<"ms"<< std::endl;
		}
	}

	if (NULL == pTestResult)
	{
		MessageBox(NULL, "No TestResult found", TEXT("Message"), 0);
		return 3;
	}

	//teststep
	std::string sTestStepResults = "";
	for (std::map<std::string, std::string>::iterator i = pTestResult->mapStepResult.begin(); i != pTestResult->mapStepResult.end(); i++)
	{
		sTestStepResults += i->first + ":"+i->second+"\n";
	}
	MessageBox(NULL, sTestStepResults.c_str(), TEXT("TestStep"), 0);

	//display bin codes and SNR value
	std::string strSNRValue = std::to_string(pTestResult->snrValue);
	if (0 != pTestResult->huaweiSnrValue)
	{
		strSNRValue += "   " + std::to_string(pTestResult->huaweiSnrValue);
	}
	MessageBox(NULL, strSNRValue.c_str(), TEXT("SNR"), 0);


	std::string sBincodes = "";
	for (size_t i = 0; i < pTestResult->listOfBincodes.size(); i++)
	{
		sBincodes += pTestResult->listOfBincodes[i] + ", ";
	}
	MessageBox(NULL, sBincodes.c_str(), TEXT("BinCodes"), 0);

	site->WriteLog(sLogFilePath, sLogFileName);

	site->Close();
	delete site;
	site = NULL;
	
	return 0;
}