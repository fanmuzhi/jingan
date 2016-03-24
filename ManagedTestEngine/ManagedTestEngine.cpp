// This is the main DLL file.

#include "stdafx.h"

#include "ManagedTestEngine.h"
//#include <msclr/marshal_cppstd.h>

#using <system.drawing.dll>

using namespace System::Runtime::InteropServices;
using namespace System::Drawing;


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
	SynapticsSite::SynapticsSite(UInt16 siteNumber, UInt32 devSerNum, String^ configPath, SynapticsAdcBaseLineInfo^ adcInfo)
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
		UInt32 rc = Syn_Site::CreateSiteInstance(siteNum, devSN, path, adcBaseLineInfo, psite);
		if (rc != 0)
		{
			throw exception("Construct site failed.");
		}
		site = psite;
	}

	SynapticsSite::~SynapticsSite()
	{
		if (NULL != site)
		{
			delete site;
			site = NULL;
		}
	}

	UInt32 SynapticsSite::Open()
	{
		return site->Open();
	}

	UInt32 SynapticsSite::ExecuteTestStep(String^ testName)
	{
		std::string testStepName;
		MarshalString(testName, testStepName);
		return site->ExecuteTestStep(testStepName);
	}

	UInt32 SynapticsSite::GetTestResult(SynapticsTestResult^ %synaTestResult)
	{
		Syn_DutTestResult* ptestResult;
		UInt32 rc = site->GetTestResult(ptestResult);
		if (rc != 0)
		{
			return rc;
		}

		//serial number
		synaTestResult->SensorSerialNumber = gcnew String(ptestResult->_sSensorSerialNumber.c_str());

		//each test step and test result
		for (std::map<std::string, std::string>::iterator i = ptestResult->_mapTestPassInfo.begin(); i != ptestResult->_mapTestPassInfo.end(); i++)
		{
			String^ key = gcnew String(i->first.c_str());
			String^ value = gcnew String(i->second.c_str());
			if (!synaTestResult->StepResult->ContainsKey(key))
			{
				synaTestResult->StepResult->Add(key, value);
			}
		}

		//bin codes
		synaTestResult->BinCodes->Clear();
		for (auto i = 0; i < ptestResult->_binCodes.size(); i++)
		{
			String^ bincode = gcnew String(ptestResult->_binCodes[i].c_str());
			synaTestResult->BinCodes->Add(bincode);
		}

		//snr
		synaTestResult->SNR = ptestResult->_snrResults.SNR[6];

		//no finger image
		synaTestResult->ImageNoFinger->Clear();
		for (auto i = 0; i < ptestResult->_acqImgNoFingerResult.iRealRowNumber; i++)
		{
			for (auto j = 0; j < ptestResult->_acqImgNoFingerResult.iRealColumnNumber; j++)
			{
				synaTestResult->ImageNoFinger->Add(ptestResult->_acqImgNoFingerResult.arr_ImageFPSFrame.arr[i][j]);
			}
		}
		synaTestResult->ImageNoFingerRow = ptestResult->_acqImgNoFingerResult.iRealRowNumber;
		synaTestResult->ImageNoFingerCol = ptestResult->_acqImgNoFingerResult.iRealColumnNumber;

		//finger image
		synaTestResult->ImageFinger->Clear();
		for (auto i = 0; i < ptestResult->_acqImgFingerResult.iRealRowNumber; i++)
		{
			for (auto j = 0; j < ptestResult->_acqImgFingerResult.iRealColumnNumber; j++)
			{
				synaTestResult->ImageFinger->Add(ptestResult->_acqImgFingerResult.arr_ImageFPSFrame.arr[i][j]);
			}
		}
		synaTestResult->ImageFingerRow = ptestResult->_acqImgFingerResult.iRealRowNumber;
		synaTestResult->ImageFingerCol = ptestResult->_acqImgFingerResult.iRealColumnNumber;
		return rc;
	}

	UInt32 SynapticsSite::Close()
	{
		return site->Close();
	}

	List<String^>^ SynapticsSite::GetTestStepList()
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

	void SynapticsSite::WriteLog(String^ path, String^ fileName)
	{
		std::string logPath, fname;
		MarshalString(path, logPath);
		MarshalString(fileName, fname);
		site->Write_Log(logPath, fname);
	}

	//
	//Managed DeviceManager
	//
	SynapticsDeviceManager::SynapticsDeviceManager()
	{
		deviceManager = NULL;
		deviceManager = new Syn_DeviceManager();
	}

	SynapticsDeviceManager::~SynapticsDeviceManager()
	{
		if (NULL != deviceManager)
		{
			delete deviceManager;
			deviceManager = NULL;
		}
	}

	UInt32 SynapticsDeviceManager::Open()
	{
		return deviceManager->Open();
	}

	List<UInt32>^ SynapticsDeviceManager::GetSNList()
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

	UInt32 SynapticsDeviceManager::UpdateFW()
	{
		return deviceManager->UpdateFirmware();
	}

	List<UInt32>^ SynapticsDeviceManager::UpdateADCOffsets(UInt32 serialnumber, UInt32 vdd, UInt32 vio, UInt32 vled, UInt32 vddh)
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

	UInt32 SynapticsDeviceManager::SetLED(UInt32 serialnumber)
	{
		return deviceManager->SetLeds(serialnumber);
	}

	UInt32 SynapticsDeviceManager::Close()
	{
		return deviceManager->Close();
	}
}
