#pragma once

//local
#include "sjimportexport.h"
#include "Synaptics_Utils.h"

//std
#include <stdint.h>
#include <vector>
#include <string>

class Syn_DeviceManager;


class SYNA_SJ_API Synaptics_DeviceManage
{

public:

	Synaptics_DeviceManage();
	~Synaptics_DeviceManage();

	uint32_t Open();

	std::vector<uint32_t> GetSerialNumberList();

	uint32_t UpdateFirmware();

	uint32_t UpdateADCOffsets(uint32_t serialnumber, uint32_t vdd, uint32_t vio, uint32_t vled, uint32_t vddh, uint32_t arAdcBaseLines[4][4]);
		
	uint32_t SetLED(uint32_t serialnumber);

	uint32_t Close();

private:

	Syn_DeviceManager *_pSyn_DeviceManager;
};

