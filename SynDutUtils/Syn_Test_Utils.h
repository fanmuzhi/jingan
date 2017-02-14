#pragma once

#include <stdint.h>
#include <vector>
#include <string>

using namespace std;

#define MPC04_CLOCKRATE			8
#define M5_CLOCKRATE			15

#define	TESTENGINE_COUNTS_MAX	20

struct FpFrame
{
	int16_t arrImage[160][160];
};

enum TestDataType{ Initialization, ROMTest, RAMTest, ProgrammingMFW, MFWCheck, ProgrammingIOTA, CurrentTest, SleepCurrentTest, DRdyTest, WOF_Baseline, Calibrate, AcqImgNoFinger, WaitStimulus, AcqImgFinger, WOF_Signal, SNRTest, BubbleTest, Imperfections, IOTACheck, Finalization };

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
	int16_t FWBaseline[10000];
	int16_t LNABaseline[10000];
};