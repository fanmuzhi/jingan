#pragma once

#include "syn_devicemanager.h"

#include <windows.h>
#include <winnt.h>
#include <winusb.h>

#include <stdint.h>

//std
#include <vector>
#include <string>
#include <iostream>
using namespace std;

#define DEFAULT_TIMEOUT_VALUE 500

class SYNA_BRIDGE_API syn_bridge
{
public:

	syn_bridge(string serialnumber);

	virtual ~syn_bridge();

	static uint32_t CreateDeviceInstance(string serialnumber, devicetype type, syn_bridge * &opBridgeInstance);

	virtual uint32_t Init() = 0;

	virtual uint32_t SetPortSPI(uint32_t clockRate, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	virtual uint32_t GetIdentity(uint32_t *identifierp, uint32_t *boot_versionp, uint32_t *appl_versionp, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	virtual uint32_t SetVoltages(unsigned int vcc, unsigned int spivcc, uint32_t timeout = DEFAULT_TIMEOUT_VALUE, unsigned int sleeptime = 20, bool vccfirst = true, unsigned int middelaytime = 0) = 0;

	virtual uint32_t GPIO_CheckDRDY(bool *drdyp, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	//virtual uint32_t GPIO_SetDRDY(bool drdyval, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	virtual uint32_t GPIO_CheckSleepN(bool *sleepval, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	virtual uint32_t GPIO_SetSleepN(bool sleepval, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	virtual uint32_t GPIO_CheckSwitch(bool *switchval, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	//virtual uint32_t GPIO_SetSwitch(bool switchval, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	virtual uint32_t GPIO_CheckOSCTrim(bool *trimchval, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	virtual uint32_t GPIO_SetOSCTrim(bool trimval, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0;

	virtual uint32_t GetCurrentValues(uint32_t arrValues[2], bool lowGain = true, uint32_t timeout = DEFAULT_TIMEOUT_VALUE) = 0; //arrValues - [0]:1.8,[1]:3.3

	virtual uint32_t Write(const uint8_t *datap, unsigned int nbytes, bool deassert_mcs_after) = 0;

	virtual uint32_t Read(uint8_t *datap, unsigned int nbytes, bool deassert_mcs_after) = 0;

protected:

	bool assigndevice();

	static syn_devicemanager *_devicemanager;

	string _serialnumber;

protected:
};