#pragma once

//local
#include "Synaptics_Utils.h"

//std
#include <stdint.h>
#include <vector>
#include <string>

class syn_devicemanager;

class SYNA_SJ_API Synaptics_DLLVersion
{
public:
	static std::string GetDLLVersion();
};

class SYNA_SJ_API Synaptics_DeviceManage
{

public:

	Synaptics_DeviceManage();
	~Synaptics_DeviceManage();

	uint32_t Open(Syn_DeviceType DeviceType);

	uint32_t GetSerialNumberList(std::vector<std::string> &oListOfSerialNumber);

	uint32_t UpdateADCOffsets(Syn_DeviceType DeviceType, std::string serialnumber, uint32_t vdd, uint32_t vio, uint32_t vled, uint32_t vddh, uint32_t arrAdcBaseLines[4]);

	uint32_t Close();

private:

	syn_devicemanager *_pSynDeviceManager;
};