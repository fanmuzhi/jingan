#pragma once

#ifdef FPBRIDGE_EXPORTS
	#define SYNA_BRIDGE_API __declspec(dllexport)
#else
	#define SYNA_BRIDGE_API __declspec(dllimport)
#endif

#include <windows.h>
#include <winnt.h>
#include <winusb.h>
#include <SetupAPI.h>

#include <stdint.h>

#include <string>
#include <vector>
using namespace std;

enum devicetype{ spi_mpc04, spi_m5 };

typedef struct
{
	/*
		deviceHandle:mpc04 or ftdi/m5;
		mpc04:use deviceHandle to retrieve USBHANDLE;
		m5:use deviceHandle(FT_HANDLE).
	*/
	//HANDLE					deviceHandle;
	vector<HANDLE>			deviceHandleList;

	string					serialnumber;
	bool					assign;
	WINUSB_INTERFACE_HANDLE usbHandle;

	UCHAR deviceSpeed;
	UCHAR bulkInPipe;
	UCHAR bulkOutPipe;
	UCHAR interruptPipe;
}DeviceInfo;

typedef std::vector<DeviceInfo*> ListOfPtrDeviceInfo;

class SYNA_BRIDGE_API syn_devicemanager
{

public:

	friend class syn_bridge;

	syn_devicemanager();
	~syn_devicemanager();

	uint32_t Connect(devicetype iDeviceType);

	uint32_t GetSerialNumberList(std::vector<std::string> &oSerialNumberList);

	ListOfPtrDeviceInfo * GetDeviceList();

	uint32_t Disconnect();

protected:

	uint32_t mpc_deviceinit();

	BOOL mpc_retrieveinfo(LPCTSTR devicePath, DeviceInfo &oDeviceInfo);

	uint32_t m5_deviceinit();

	//device manage
	bool detectdevice(string sSerialNumber);

	bool assigndevice(string sSerialNumber);

	uint32_t releasedevice(string sSerialNumber);

private:

	ListOfPtrDeviceInfo *_pListOfPtrDeviceInfo;
};

