#pragma once

#ifdef FPBRAVOMODULE_EXPORTS
	#define SYNA_FPBRAVOMODULE_API __declspec(dllexport)
#else
	#define SYNA_FPBRAVOMODULE_API __declspec(dllimport)
#endif

#include "syn_bridge.h"

#define TIMEOUT_VALUE 2000

//error code
#define ERROR_TIME_OUT				0x1500
#define ERROR_CRC_VERIFY			0x1501
#define ERROR_BL_MODE				0x1502
#define ERROR_PARAM_UNDEFINE		0x1502
#define ERROR_TYPE					0x1503
#define ERROR_PARAMETER				0x1504

typedef enum{ OFF_REPLYSENT, CMDWAIT, CMDPROC, REPLY };

typedef enum{ UNKNOWN, ABSENT, STILL, MOVING };

typedef struct Bravo_Sensor_Status
{
	unsigned int SOFTSTATE;
	unsigned int SOFTSTATE_EPSTATE;
	unsigned int SOFTSTATE_FPSTATE;
	unsigned int SOFTSTATE_SSLSTATE;
	unsigned int EP2FLUSH;//Unused in Denali/Hayes
	unsigned int JUSTWOKE;
	unsigned int STATECHANGED;
	unsigned int EP2INSIZE;//Unused in Denali/Hayes
	unsigned int EP2INDONE;//Unused in Denali/Hayes
	unsigned int RUNNING;
	unsigned int EP1OUT;
	unsigned int EP1IN;
	unsigned int DRDY;
	unsigned int JUSTRESET;
	unsigned int ALIVE;
	unsigned int EP1INSIZE;//Only in Denali/Hayes
	unsigned int SOFTSTATE2;//Only in Denali/Hayes
	//unsigned int SOFTSTATE2_ACQNUM;//Only in Denali/Hayes
	//unsigned int SOFTSTATE2_EVNUM;//Only in Denali/Hayes
}Bravo_Sensor_Status_t;

enum class FlashReadType{ MISSION_FW, IOTA, FIB };
enum class FlashReadFlags{ BIGGEST, SHA256 };

enum class FlashEraseType{ MISSION_FW, IOTA, MF_IOTA, FIB, SDB, ALL };

enum class CalibrateType{ Firmware, Patch };

enum class StateGetType{ FW_BL, LNA_BL, DM, CDM, REG32BLK, REG16BLK, DIMS, ACQOPT, XSREG8BLK, STRIDEBLK, STATS };

enum class IotaFindType{ ReplyLength, GetData };

enum class WOFCalibrateType{ Baseline, Signal };

class SYNA_FPBRAVOMODULE_API FpBravoModule
{

public:

	enum BravoSensorType{ Denali, Hayes, Shasta };

	static uint32_t CreateModuleInstance(BravoSensorType Type, syn_bridge *ipSynBridge, FpBravoModule * &opBravoModule);

	FpBravoModule();
	virtual ~FpBravoModule();

	void SetBridge(syn_bridge *ipSynBridge);

	virtual uint32_t PowerOn(uint32_t vcc, uint32_t spivcc, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t PowerOff(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpTidleSet(uint16_t idletime = 0, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetVersion(uint8_t *arrVersion, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetStartInfo(uint8_t *arrStartInfo, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpLoadPatch(uint8_t* pPatch, uint32_t numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpFlashRead(FlashReadType Type, FlashReadFlags Flags, uint8_t *arrFlash, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpFlashErase(FlashEraseType Type, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpEnterBLMode(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpExitBLMode(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpReset();

	virtual uint32_t FpCalibrate(CalibrateType Type, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpStateGet(StateGetType Type, uint8_t *arr, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpFrameRead(uint8_t *arrImage, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpFrameFinish(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetImage(uint8_t *arrImage, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpUpdateADCOffsets(uint32_t arrAdcbaselines[4], uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpIotafind(IotaFindType Type, uint32_t &replylength, uint8_t *arrIotaData, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	/*
		type : IOTA_ITYPE
		BIN(...BASE.bin etc.) include type, type default value set to zero!	
	*/
	virtual uint32_t FpIotawrite(uint8_t *arrIotadata, uint32_t size, uint16_t type, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpPeek(uint32_t address, uint8_t opsize, uint32_t &ovalue, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpPoke(uint32_t address, uint32_t value, uint8_t opsize = 4, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpTestRun(uint8_t patchcmd, uint8_t *arrResponse, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpWOFBaseline(uint8_t *arrResponse, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpWOFSignal(uint8_t* arrCMD, uint32_t numBytes, uint8_t *arrResponse, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t checkBLMode(bool &mode) = 0;

protected:

	uint32_t getStatus(Bravo_Sensor_Status_t &oSensorStatus);

	//void printStatus(Bravo_Sensor_Status_t sensorStatus);

	uint32_t executeCmd(uint8_t cmdname, uint8_t *cmdbufp, uint32_t buflen, uint8_t *replybufp, uint32_t replybuflen, bool crc, uint16_t &replystatus, uint32_t timeout);

	uint32_t writeCmd(uint8_t cmdname, uint8_t *cmdbufp, uint32_t buflen, bool crc, uint32_t timeout);

	uint32_t readCmd(uint8_t *arrRep, uint32_t size, bool crc, uint32_t timeout);

	uint32_t crc32_calc(const uint8_t *datap, unsigned int nbytes, uint32_t crc);

protected:

	syn_bridge *_pSynBridge;
};

