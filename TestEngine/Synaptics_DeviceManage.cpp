//Local
#include "Syn_DeviceManager.h"

#include "Synaptics_DeviceManage.h"
#include "Syn_Exception.h"

Synaptics_DeviceManage::Synaptics_DeviceManage()
:_pSyn_DeviceManager(NULL)
{
	_pSyn_DeviceManager = new Syn_DeviceManager();
}

Synaptics_DeviceManage::~Synaptics_DeviceManage()
{
	if (NULL != _pSyn_DeviceManager)
	{
		delete _pSyn_DeviceManager;
		_pSyn_DeviceManager = NULL;
	}
}

uint32_t Synaptics_DeviceManage::Open()
{
	if (NULL == _pSyn_DeviceManager)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	else
	{
		return _pSyn_DeviceManager->Open();
	}

}

std::vector<uint32_t> Synaptics_DeviceManage::GetSerialNumberList()
{
	std::vector<uint32_t> listOfSerialNumber;
	listOfSerialNumber.clear();

	if (NULL != _pSyn_DeviceManager)
	{
		_pSyn_DeviceManager->GetSerialNumberList(listOfSerialNumber);
	}

	return listOfSerialNumber;
}

uint32_t Synaptics_DeviceManage::UpdateFirmware()
{
	if (NULL == _pSyn_DeviceManager)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	else
	{
		return _pSyn_DeviceManager->UpdateFirmware();
	}
}

uint32_t Synaptics_DeviceManage::UpdateADCOffsets(uint32_t serialnumber, uint32_t vdd, uint32_t vio, uint32_t vled, uint32_t vddh, uint32_t arAdcBaseLines[4][4])
{
	if (NULL == _pSyn_DeviceManager)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	else
	{
		return _pSyn_DeviceManager->UpdateADCOffsets(serialnumber, vdd, vio,vled,vddh,arAdcBaseLines);
	}
}

uint32_t Synaptics_DeviceManage::SetLED(uint32_t serialnumber)
{
	if (NULL == _pSyn_DeviceManager)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	else
	{
		return _pSyn_DeviceManager->SetLeds(serialnumber);
	}
}

uint32_t Synaptics_DeviceManage::Close()
{
	if (NULL == _pSyn_DeviceManager)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	else
	{
		return _pSyn_DeviceManager->Close();
	}
}