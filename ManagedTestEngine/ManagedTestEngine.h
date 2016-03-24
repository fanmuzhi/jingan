// ManagedTestEngine.h

#pragma once

#include "Syn_Site.h"
#include "Syn_DeviceManager.h"


using namespace System;
using namespace System::Collections::Generic;

namespace ManagedTestEngine {

	//
	//Managed AdcBaseLineInfo
	//
	public ref struct SynapticsAdcBaseLineInfo
	{
		//array<uint32_t, 2>^ m_arAdcBaseLines = gcnew array<uint32_t,2>(4,4);
		List<UInt32>^ m_arAdcBaseLines = gcnew List<UInt32>();
		UInt32 m_nVdd;
		UInt32 m_nVio;
		UInt32 m_nVled;
		UInt32 m_nVddh;
	};


	//
	//Managed TestResult
	//
	public ref struct SynapticsTestResult
	{
		Dictionary<String^, String^>^ StepResult = gcnew Dictionary<String^, String^>();

		String^ SensorSerialNumber = gcnew String("");

		List<Double>^ SNRValues = gcnew List<Double>();

		List<String^>^ BinCodes = gcnew List<String^>();

		List<uint8_t>^ ImageNoFinger = gcnew List<uint8_t>();
		uint8_t ImageNoFingerRow;
		uint8_t ImageNoFingerCol;
		//uint8_t arrImageNoFinger[200][200];
		//int imageNoFingerRows;
		//int imageNoFingerCols;

		List<uint8_t>^ ImageFinger = gcnew List<uint8_t>();
		uint8_t ImageFingerRow;
		uint8_t ImageFingerCol;
		//uint8_t arrImageFinger[200][200];
		//int imageFingerRows;
		//int imageFingerCols;

		//uint32_t   buildtime;        /* Unix-style build time, in seconds   *//*  from 1/1/1970 12:00 AM GMT         */
		//uint32_t   buildnum;         /* build number                        */
		//uint8_t    vmajor;           /* major version                       */
		//uint8_t    vminor;           /* minor version                       */
		//uint8_t    target;           /* target, e.g. VCSFW_TARGET_ROM       */
		//uint8_t    product;          /* product, e.g.  VCSFW_PRODUCT_FALCON */
		//uint8_t    siliconrev;       /* silicon revision                    */
		//uint8_t    formalrel;        /* boolean: non-zero -> formal release */
		//uint8_t    platform;         /* Platform (PCB) revision             */
		//uint8_t    patch;            /* patch level                         */
		//uint8_t    serial_number[6]; /* 48-bit Serial Number                */
		//uint8_t    security[2];      /* bytes 0 and 1 of OTP                */
		//uint32_t   patchsig;         /* opaque patch signature              */
		//uint8_t    iface;            /* interface type, see below           */
		//uint8_t    otpsig[3];        /* OTP Patch Signature                 */
		//uint16_t   otpspare1;        /* spare space                         */
		//uint8_t    reserved;         /* reserved byte                       */
		//uint8_t    device_type;      /* device type                         */
		//List<uint8_t>^ getVerInfo = gcnew List<uint8_t>();

		//OTP content
		//uint8_t arrBootSector0[BS0_SIZE];
		//uint8_t arrBootSector1[BS1_SIZE];

		//uint8_t arrMainSector0[2048];
		//uint8_t arrMainSector1[2048];
	};


	//
	//Managed Site
	//
	public ref class SynapticsSite
	{
	public:
		SynapticsSite(UInt16 siteNumber, UInt32 devSerNum, String^ configPath, SynapticsAdcBaseLineInfo^ adcInfo);
		~SynapticsSite();

		UInt32 Open();
		UInt32 ExecuteTestStep(String^ testName);
		UInt32 GetTestResult(SynapticsTestResult^ %testResult);
		UInt32 Close();
		List<String^>^ GetTestStepList();
		void WriteLog(String^ path, String^ fileName);

	private:
		Syn_Site * site;

	};

	//
	//Managed DeviceManager
	//
	public ref class SynapticsDeviceManager
	{
	public:
		SynapticsDeviceManager();
		~SynapticsDeviceManager();

		UInt32 Open();
		List<UInt32>^ GetSNList();
		UInt32 UpdateFW();
		List<UInt32>^ UpdateADCOffsets(UInt32 serialnumber, UInt32 vdd, UInt32 vio, UInt32 vled, UInt32 vddh);
		UInt32 SetLED(UInt32 serialnumber);
		UInt32 Close();

	private:
		Syn_DeviceManager * deviceManager;
	};
}
