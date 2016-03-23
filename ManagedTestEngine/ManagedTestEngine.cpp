// This is the main DLL file.

#include "stdafx.h"

#include "ManagedTestEngine.h"
//#include <msclr/marshal_cppstd.h>
using namespace System::Runtime::InteropServices;


namespace ManagedTestEngine {

	//System::String to std::string
	void MarshalString (String^ s, string& os) 
	{
	   const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	   os = chars;
	   Marshal::FreeHGlobal(IntPtr((void*)chars));
	}
	void MarshalString (String^ s, wstring& os) 
	{
	   const wchar_t* chars = (const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
	   os = chars;
	   Marshal::FreeHGlobal(IntPtr((void*)chars));
	}

	//
	//Managed Site
	//
	OFilmSite::OFilmSite(UInt16 siteNumber, UInt32 devSerNum, String^ configPath, OFilmAdcBaseLineInfo^ adcInfo)
		:site(NULL)
	{
		Syn_Site * psite = NULL;
		AdcBaseLineInfo adcBaseLineInfo;

		adcBaseLineInfo.m_bExecuted = 1;
		adcBaseLineInfo.m_nVdd = adcInfo->m_nVdd;
		adcBaseLineInfo.m_nVddh = adcInfo->m_nVddh;
		adcBaseLineInfo.m_nVio = adcInfo->m_nVio;
		adcBaseLineInfo.m_nVled = adcInfo->m_nVled;
		
		int k = 0;
		for (int i = 0; i < NUM_CURRENT_VALUES; i++)
		{
			for (int j = 0; j < KNUMGAINS; j++)
			{
				adcBaseLineInfo.m_arAdcBaseLines[i][j] = adcInfo->m_arAdcBaseLines[k];
				k++;
			}
		}

		uint8_t siteNum = siteNumber;
		uint32_t devSN = devSerNum;
 
		std::string path;
		MarshalString(configPath, path);
		Syn_Site::CreateSiteInstance(siteNum, devSN, path, adcBaseLineInfo, psite);
		site = psite;
	}

	OFilmSite::~OFilmSite()
	{
		if (NULL != site)
		{
			delete site;
			site = NULL;
		}
	}

	UInt32 OFilmSite::Open()
	{
		return site->Open();
	}

	UInt32 OFilmSite::ExecuteTestStep(String^ testName)
	{
		std::string testStepName;
		MarshalString(testName, testStepName);
		return site->ExecuteTestStep(testStepName);
	}

	UInt32 OFilmSite::GetTestResult(OFilmTestResult^ %ofilmTestResult)
	{
		Syn_DutTestResult* ptestResult;
		UInt32 rc = site->GetTestResult(ptestResult);
		if (rc != 0)
		{
			return rc;
		}

		ofilmTestResult->SensorSerialNumber = gcnew String(ptestResult->_sSensorSerialNumber.c_str());

		for (std::map<std::string, std::string>::iterator i = ptestResult->_mapTestPassInfo.begin(); i != ptestResult->_mapTestPassInfo.end(); i++)
		{
			String^ key = gcnew String(i->first.c_str());
			String^ value = gcnew String(i->second.c_str());
			if (!ofilmTestResult->StepResult->ContainsKey(key))
			{
				ofilmTestResult->StepResult->Add(key, value);
			}
		}

		return rc;
	}

	UInt32 OFilmSite::Close()
	{
		return site->Close();
	}

	List<String^>^ OFilmSite::GetTestStepList()
	{
		vector<string> testSteps;
		site->GetTestStepList(testSteps);
		List<String^>^ testStepList = gcnew List<String^>();

		for (int i = 0; i < testSteps.size(); i++)
		{
			String^ step = gcnew String(testSteps[i].c_str());
			testStepList->Add(step);
		}
		testSteps.clear();
		return testStepList;
	}

	void OFilmSite::WriteLog(String^ path, String^ fileName)
	{
		std::string logPath, fname;
		MarshalString(path, logPath);
		MarshalString(fileName, fname);
		site->Write_Log(logPath, fname);
	}

	//
	//Managed DeviceManager
	//
	OFilmDeviceManager::OFilmDeviceManager()
	{
		deviceManager = NULL;
		deviceManager = new Syn_DeviceManager();
	}

	OFilmDeviceManager::~OFilmDeviceManager()
	{
		if (NULL != deviceManager)
		{
			delete deviceManager;
			deviceManager = NULL;
		}
	}

	UInt32 OFilmDeviceManager::Open()
	{
		return deviceManager->Open();
	}

	List<UInt32>^ OFilmDeviceManager::GetSNList()
	{
		List<UInt32>^ snlist = gcnew List<UInt32>();
		
		std::vector<uint32_t> serialnumberList;
		deviceManager->GetSerialNumberList(serialnumberList);

		for (size_t i = 1; i <= serialnumberList.size(); i++)
		{
			snlist->Add(serialnumberList[i - 1]);
		}
		serialnumberList.clear();
		return snlist;
	}

	UInt32 OFilmDeviceManager::UpdateFW()
	{
		return deviceManager->UpdateFirmware();
	}

	List<UInt32>^ OFilmDeviceManager::UpdateADCOffsets(UInt32 serialnumber, UInt32 vdd, UInt32 vio, UInt32 vled, UInt32 vddh)
	{
		List<UInt32>^ adclist = gcnew List<UInt32>();

		uint32_t arAdcBaseLines[NUM_CURRENT_VALUES][KNUMGAINS] = { 0 };
		deviceManager->UpdateADCOffsets(serialnumber, vdd, vio, vled, vddh, arAdcBaseLines);
		for (int i = 0; i < NUM_CURRENT_VALUES; i++)
		{
			for (int j = 0; j < KNUMGAINS; j++)
			{
				adclist->Add(arAdcBaseLines[i][j]);
			}
		}
		return adclist;
	}

	UInt32 OFilmDeviceManager::SetLED(UInt32 serialnumber)
	{
		return deviceManager->SetLeds(serialnumber);
	}

	UInt32 OFilmDeviceManager::Close()
	{
		return deviceManager->Close();
	}
}
