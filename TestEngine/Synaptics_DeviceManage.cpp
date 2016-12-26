//Local
#include "syn_devicemanager.h"
#include "syn_bridge.h"
#include "Synaptics_DeviceManage.h"
#include "SYN_TestUtils.h"
#include "Syn_Exception.h"

Synaptics_DeviceManage::Synaptics_DeviceManage()
:_pSynDeviceManager(NULL)
{
	_pSynDeviceManager = new syn_devicemanager();
}

Synaptics_DeviceManage::~Synaptics_DeviceManage()
{
	if (NULL != _pSynDeviceManager)
	{
		delete _pSynDeviceManager;
		_pSynDeviceManager = NULL;
	}
}

uint32_t Synaptics_DeviceManage::Open(Syn_DeviceType DeviceType)
{
	if (NULL == _pSynDeviceManager)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	
	uint32_t rc(0);

	devicetype Type;
	switch (DeviceType)
	{
		case MPC04:
			Type = spi_mpc04;
			break;
		case M5:
			Type = spi_m5;
			break;
		default:
			Type = spi_m5;
			break;
	}

	rc = _pSynDeviceManager->Connect(Type);

	return rc;
}

uint32_t Synaptics_DeviceManage::GetSerialNumberList(std::vector<std::string> &oListOfSerialNumber)
{
	oListOfSerialNumber.clear();

	if (NULL == _pSynDeviceManager)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}

	uint32_t rc = _pSynDeviceManager->GetSerialNumberList(oListOfSerialNumber);

	return rc;
}

uint32_t Synaptics_DeviceManage::UpdateADCOffsets(Syn_DeviceType DeviceType, std::string serialnumber, uint32_t vdd, uint32_t vio, uint32_t vled, uint32_t vddh, uint32_t arAdcBaseLines[4])
{
	devicetype Type;
	uint32_t clockRate(MPC04_CLOCKRATE);
	switch (DeviceType)
	{
		case MPC04:
			Type = spi_mpc04;
			clockRate = MPC04_CLOCKRATE;
			break;
		case M5:
			Type = spi_m5;
			clockRate = M5_CLOCKRATE;
			break;
		default:
			Type = spi_m5;
			clockRate = M5_CLOCKRATE;
			break;
	}
	
	uint32_t rc(0);

	if (NULL != _pSynDeviceManager)
	{
		delete _pSynDeviceManager;
		_pSynDeviceManager = NULL;
	}

	syn_bridge *pBridge = NULL;
	rc = syn_bridge::CreateDeviceInstance(serialnumber, Type, pBridge);
	if (0 == rc&&NULL != pBridge)
	{
		pBridge->SetPortSPI(clockRate);
		pBridge->SetVoltages(vddh, vdd);

		uint32_t arrLowGain[2] = { 2 };
		uint32_t arrHighGain[2] = { 2 };
		rc = pBridge->GetCurrentValues(arrLowGain);
		rc = pBridge->GetCurrentValues(arrHighGain, false);

		arAdcBaseLines[0] = arrLowGain[0];
		arAdcBaseLines[1] = arrLowGain[1];
		arAdcBaseLines[2] = arrHighGain[0];
		arAdcBaseLines[3] = arrHighGain[1];

		pBridge->SetVoltages(0, 0);
		delete pBridge;
		pBridge = NULL;
	}

	return rc;
}

uint32_t Synaptics_DeviceManage::Close()
{
	if (NULL == _pSynDeviceManager)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	else
	{
		return _pSynDeviceManager->Disconnect();
	}
}

std::string Synaptics_DLLVersion::GetDLLVersion()
{
	return SW_VERSION;
}
