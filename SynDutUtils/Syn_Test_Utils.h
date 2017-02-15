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

struct CalibrateTestData : public SynTestData
{
	//parameter

	//result
	int16_t FWBaseline[BRAVO_IMAGE_RAWDATA_MAX];
	int16_t LNABaseline[BRAVO_IMAGE_RAWDATA_MAX];
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