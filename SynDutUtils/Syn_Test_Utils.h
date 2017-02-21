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
#define MAXROW 200
#define MAXCOL 200

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

#define BRAVO_WOF_DATA_MAX			20
#define BRAVO_WOF_COUNTS_MAX		5

struct WOF_BaselineTestData : public SynTestData
{
	//parameter
	int gainNumbers;

	//result
	uint16_t arrWOFBaseline[BRAVO_WOF_COUNTS_MAX];
	uint16_t arrWOFOffset[BRAVO_WOF_COUNTS_MAX];
};

struct WOF_SignalTestData : public SynTestData
{
	//parameter
	int lowSignalLimit;
	int highSignalLimit;

	//result
	uint16_t arrWOFSignal[BRAVO_WOF_COUNTS_MAX];

	uint16_t selectedGain, selectedOffset, selectedSingal;
};

struct WaitStilumusTestData : public SynTestData
{
	//parameter

	//result
	int16_t FrameData[BRAVO_IMAGE_RAWDATA_MAX];
};

struct SNRTestData : public SynTestData
{
	//simply : waiting for new snr algorithm release

	//parameter
	uint32_t snrLimit;
	uint32_t signalLimit;
	uint32_t noiseLimit;

	//result
	double   snrValue;
	uint32_t signalValue;
	double   noiseValue;
};

struct ImperfectionsTestData : public SynTestData
{
	//parameter
	int m_peggedThreshold;
	int m_flooredThreshold;
	int m_maxAdjacentPixelsAllowed;
	int numFrames;

	//result
	int avg[MAXROW][MAXCOL];
	int consecutive_pegged_rows[MAXROW - 2];
	int consecutive_floored_rows[MAXROW - 2];
	int consecutive_pegged_cols[MAXCOL - 3];
	int consecutive_floored_cols[MAXCOL - 3];

	int floored_ROW;
	int pegged_ROW;
	int floored_COL;
	int pegged_COL;
};

struct SharpnessData : public SynTestData
{
	//parameter
	int  limit;

	//result
	float SHARPNESS[4];//SHARPNESS[0]->Z1, SHARPNESS[1]->Z2, SHARPNESS[2]->Z3, SHARPNESS[3]->OVERALL
	int	  Gx[MAXROW][MAXCOL];
	int	  Gy[MAXROW][MAXCOL];
	int	  h[MAXCOL];
	int	  g[MAXROW][MAXCOL];
	int	  pImg_t[MAXROW][MAXCOL];
	float percent;
};

struct FinalizationTestData : public SynTestData
{
	//parameter

	//result

};