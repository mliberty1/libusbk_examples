/* Copyright (c) 2014 Jetperch LLC.  See LICENSE.txt */

/*!
 * \file
 * 
 * \brief Rapidly switch device alternate settings.
 *
 * This examples was created to isolate and demonstate a WDF_Violation and 
 * Blue Screen of Death (BSoD).  The BSoD appears to occur when the device
 * is removed (unplugged) while switching alternate settings.  The original
 * problem was presumed to be when switching away from an altsetting with
 * streaming isochronous data, but this example shows that the BSoD occurs
 * when just switching alternate settings.  The altsetting does not even
 * need to contain an ISO IN endpoint.
 *
 * Testing was initially performed on a Dell Precision M4600 running 
 * Windows 8.1 with libusbk 3.0.6.
 */

#define LOG_LEVEL LOG_LEVEL_INFO
#include "log.h"

#include <Windows.h>
#include <stdio.h>
#include <libusbk.h>
#include <map>
#include <memory>
#include <exception>


//! Default example vendor id
#ifndef EXAMPLE_VID
#define EXAMPLE_VID 0x20AB
#endif

//! Default example product id
#ifndef EXAMPLE_PID
#define EXAMPLE_PID 0x0114
#endif


/*!
 * /brief Exception thrown by the Device class.
 */
class DeviceException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "DeviceException";
	}
} deviceException;

/* Forward declaration */
class Device;

/* File-scope variables. */
typedef std::map< std::string, std::shared_ptr<Device> > PathMapT;
static PathMapT pathMap;
static bool doExit_ = false;
static uint8_t altsetting = 0;

/* Console control handler - exit on any signal. */
BOOL CtrlHandler(DWORD fdwCtrlType) {
	doExit_ = true;
	return (TRUE);
}


/*!
 * \brief Handle a single connected device.
 */
class Device {
public:
	/*!
	 * \brief Construct a new instance and open the device.
	 *
	 * Normally, the device will autodetect the target alternate interface by
	 * a querying the device.  Alternatively, if ::altsetting is not 0, then
	 * it will be used.
	 *
	 * \param devinfo The devinfo handle to associate with this instance.
	 */
	Device(KLST_DEVINFO const * devinfo)
		: libusbk_devinfo(*devinfo) 
		, libusbk_handle(NULL)
		, libusbk_usb()
		, iso_altsetting_(altsetting)
		, current_altsetting_(0)
		, error_count_(0)
	{
		memset(&libusbk_usb, 0, sizeof(libusbk_usb));
		if (!LibK_LoadDriverAPI(&libusbk_usb, libusbk_devinfo.DriverID)) {
			throw deviceException;
		}
		if (!libusbk_usb.Init(&libusbk_handle, const_cast<KLST_DEVINFO*>(&libusbk_devinfo))) {
			throw deviceException;
		}
		if (!libusbk_usb.ClaimInterface(libusbk_handle, 0, FALSE)) {
			libusbk_usb.Free(libusbk_handle);
			libusbk_handle = NULL;
			throw deviceException;
		}
		if (iso_altsetting_ == 0) {
			get_iso_endpoint();
		}
	}

	~Device() {
		if (NULL != libusbk_handle) {
			libusbk_usb.ReleaseInterface(libusbk_handle, 0, FALSE);
			libusbk_usb.Free(libusbk_handle);
			libusbk_handle = NULL;
		}
	}

	/*!
	 * \brief Search all alternate interfaces for the first once which contains
	 *		an isochronous endpoint.
	 */
	void get_iso_endpoint() {
		LOGS_INFO("Device::get_iso_endpoint: finding first ISO endpoint");
		USB_INTERFACE_DESCRIPTOR interface_descriptor;
		WINUSB_PIPE_INFORMATION pipeInfo;
		uint8_t altsetting_idx = 0;
		while (libusbk_usb.QueryInterfaceSettings(libusbk_handle, altsetting_idx++, &interface_descriptor)) {
			uint8_t pipeIndex = 0;
			while (libusbk_usb.QueryPipe(libusbk_handle, interface_descriptor.bAlternateSetting, pipeIndex++, &pipeInfo)) {
				if (USB_ENDPOINT_DIRECTION_IN(pipeInfo.PipeId) && (UsbdPipeTypeIsochronous == pipeInfo.PipeType)) {
					iso_altsetting_ = interface_descriptor.bAlternateSetting;
					LOGF_INFO("   MATCH: PipeId=0x%02X PipeType=0x%02X Interval=%u MaximumPacketSize=%u",
						pipeInfo.PipeId, pipeInfo.PipeType, pipeInfo.Interval, pipeInfo.MaximumPacketSize);
					return;
				}
				else {
					LOGF_DEBUG("          PipeId=0x%02X PipeType=0x%02X Interval=%u MaximumPacketSize=%u",
						pipeInfo.PipeId, pipeInfo.PipeType, pipeInfo.Interval, pipeInfo.MaximumPacketSize);
				}
			}
		}
		LOGS_WARN("Device::get_iso_endpoint: could not find ISO endpoint");
		throw deviceException;
	}

	/*!
	 * \brief Perform periodic actions on this device.
	 *
	 * The current implementation will switch alternate interfaces between 0
	 * and the specified alternate interface.  
	 *
	 * \return true on success, false on error.
	 */
	bool perform() {
		if (error_count_ > 0) {
			// Be gentle: Do not perform if this device is already has an error.
			return false;
		}
		uint8_t next_altsetting = 0;
		if (current_altsetting_ == 0) {
			next_altsetting = iso_altsetting_;
		}
		int64_t time_frequency;
		int64_t time_start;
		int64_t time_end;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&time_frequency));
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time_start));
		if ((libusbk_handle != NULL) && !libusbk_usb.SetCurrentAlternateSetting(libusbk_handle, next_altsetting)) {
			LOGF_WARN("SetCurrentAlternateSetting failed: %08Xh", GetLastError());
			++error_count_;
			return false;
		}
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time_end));
		double result = static_cast<double>(time_end - time_start) / static_cast<float>(time_frequency);
		int32_t time_ms = static_cast<int32_t>(result * 1000.0);
		LOGF_NOTICE("SetCurrentAlternateSetting(%d) took %d ms", next_altsetting, time_ms);
		current_altsetting_ = next_altsetting;
		return true;
	}

public:
	KLST_DEVINFO const libusbk_devinfo;
	KUSB_HANDLE libusbk_handle;
	KUSB_DRIVER_API libusbk_usb;
	uint8_t iso_altsetting_;		///< The USB alternate interface ID.
	uint8_t current_altsetting_;	///< The current alternate interface ID.  Toggles between 0 and iso_altsetting_.
	int error_count_;				///< The total error count.  Since we guard against subsequent errors, this should be 0 or 1.
};

/*!
 * \brief Filter devices of interest.
 *
 * \param deviceInfo The detailed device information.
 * \return true for devices of interest, false otherwise.
 */
static bool device_filter(KLST_DEVINFO const * deviceInfo) {
	return ((deviceInfo->Common.Vid == EXAMPLE_VID) && (deviceInfo->Common.Pid == EXAMPLE_PID));
}

/*!
 * \brief Add a new device to our list of devices.
 *
 * \param deviceInfo The info for the device to add.
 * \return true on success or false on failure.
 */
static bool device_add(KLST_DEVINFO const * deviceInfo) {
	if (!device_filter(deviceInfo)) {
		return false;
	}
	auto it = pathMap.find(deviceInfo->DevicePath);
	if (it == pathMap.end()) {
		LOGF_NOTICE("device_add %s", deviceInfo->DevicePath);
		pathMap[deviceInfo->DevicePath] = std::make_shared<Device>(deviceInfo);
		return true;
	}
	return false;
}

/*!
* \brief Remove an existing device to our list of devices.
*
* \param deviceInfo The info for the device to add.
* \return true on success or false on failure.
*/
static bool device_remove(KLST_DEVINFO const * deviceInfo) {
	auto it = pathMap.find(deviceInfo->DevicePath);
	if (it != pathMap.end()) {
		LOGF_NOTICE("discover::device_remove %s", deviceInfo->DevicePath);
		pathMap.erase(it);
		return true;
	} else {
		LOGF_WARN("discovery::device_remove nonexistent device %s", deviceInfo->DevicePath);
	}
	return false;
}

/*!
 * \brief Scan the system for devices that have been added or removed.
 *
 * If new devices are found, then create Device instances and add them to 
 * our list of devices.  If devices are removed, then remove them from our
 * list of devices and destroy them.
 */
static int device_scan() {
	KLST_HANDLE deviceList = NULL;
	KLST_DEVINFO_HANDLE deviceInfo = NULL;

	// Get the device list
	if (!LstK_Init(&deviceList, KLST_FLAG_NONE)) {
		LOGS_ERR("could not initialize device list.");
		return 1;
	}

	LstK_MoveReset(deviceList);
	PathMapT localPathMap(pathMap);
	while (LstK_MoveNext(deviceList, &deviceInfo)) {
		device_add(deviceInfo);
		auto it = localPathMap.find(deviceInfo->DevicePath);
		if (it != localPathMap.end()) {
			localPathMap.erase(it);
		}
	}
	LstK_Free(deviceList);

	for (auto & it : localPathMap) {
		device_remove(&it.second->libusbk_devinfo);
	}
	return 0;
}

/// Print the command-line usage.
void usage() {
	printf("usage: altsetting.exe [--altsetting int]\n");
	printf("   altsetting    The alternate setting to use.  If not provided, then detect\n"
		   "                 the first alternate setting with an isochronous endpoint.\n");
}

/*!
 * \brief The main entry point for altsetting.exe.
 *
 * See usage() for the command-line arguments.
 */
int main(int argc, char* argv[]) {
	int return_code = 0;
	--argc;
	++argv;
	while (argc) {
		if (argc >= 2 && strcmp(argv[0], "--altsetting") == 0) {
			altsetting = atoi(argv[1]);
			argc -= 2;
			argv += 2;
		} else {
			usage();
			LOGF_ERROR("Unknown argument: %s", *argv)
				return -1;
		}
	}

	SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);
	log_filename("altsetting.log");
	try {
		while (!doExit_) {
			device_scan();
			for (auto & it : pathMap) {
				it.second->perform();
			}
		}
	} catch (DeviceException &) {
		return_code = 1;
	}
	return return_code;
}
