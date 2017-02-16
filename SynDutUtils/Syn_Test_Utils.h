#pragma once

#include <stdint.h>
#include <vector>
#include <string>

using namespace std;

#define MPC04_CLOCKRATE			8
#define M5_CLOCKRATE			15

#define	TESTENGINE_COUNTS_MAX	20

#define	SW_VERSION				"ALPHA_UNRELEASE"

#define BRAVO_IMAGE_RAWDATA_MAX 10000

struct FpFrame
{
	int16_t arrImage[160][160];
};

typedef enum TestDataType{ Initialization, ROMTest, RAMTest, ProgrammingMFW, MFWCheck, ProgrammingIOTA, CurrentTest, SleepCurrentTest, 
							DRdyTest, WOF_Baseline, Calibrate, AcqImgNoFinger, WaitStimulus, AcqImgFinger, WOF_Signal, SNRTest, BubbleTest, 
							Imperfections, IOTACheck, Finalization }
TestData_Type_t;

struct SynTestData
{
	SynTestData()
		:data_name(""), executed(false), pass(false), test_time(0){}

	virtual ~SynTestData(){}

	string	data_name;
	bool	executed;
	bool	pass;
	double	test_time;
};

struct InitializationTestData : public SynTestData
{
	//parameter

	//result
	string strSensorSerialNumber;
};

struct ProgrammingMFTestData : public SynTestData
{
	//parameter

	//result
	uint8_t FirmwareBuildNumber;//firmware, not mission firmware
	uint8_t ExistsMissionFirmwareNumber;//if not exists, set to 0
};

struct ProgrammingIOTATestData : public SynTestData
{
	//parameter
	uint8_t IOTAType;//0:bin, 1:data

	//result
	//0:"ProgrammingIOTATestData_BIN"
	//1:"ProgrammingIOTATestData_DATA"
};

struct CalibrateTestData : public SynTestData
{
	//parameter

	//result
	int16_t FWBaseline[BRAVO_IMAGE_RAWDATA_MAX];
	int16_t LNABaseline[BRAVO_IMAGE_RAWDATA_MAX];
};

struct SimpleFpFrame
{
	int16_t arrImage[BRAVO_IMAGE_RAWDATA_MAX];
};

struct AcqImageNoFingerTestData : public SynTestData
{
	//parameter
	unsigned int numFrame;

	//result
	vector<SimpleFpFrame*> ListOfImageNoFinger;//temp var : need modify

	int16_t arrImage[BRAVO_IMAGE_RAWDATA_MAX];
};

struct AcqImageFingerTestData : public SynTestData
{
	//parameter
	unsigned int numFrame;

	//result
	vector<SimpleFpFrame*> ListOfImageFinger;//temp var : need modify

	int16_t arrImage[BRAVO_IMAGE_RAWDATA_MAX];
};

struct WaitStilumusTestData : public SynTestData
{
	//parameter

	//result
	int16_t FrameData[BRAVO_IMAGE_RAWDATA_MAX];
};

struct FinalizationTestData : public SynTestData
{
	//parameter

	//result

};