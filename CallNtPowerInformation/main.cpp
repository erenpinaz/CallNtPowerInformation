#include <ntstatus.h>
#include <Windows.h>
#include <powrprof.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

// System Power Information
typedef struct _SYSTEM_POWER_INFORMATION {
	ULONG MaxIdlenessAllowed;
	ULONG Idleness;
	LONG TimeRemaining;
	UCHAR CoolingMode;
} SYSTEM_POWER_INFORMATION, *PSYSTEM_POWER_INFORMATION;

// Processor Power Informatio
typedef struct _PROCESSOR_POWER_INFORMATION {
	ULONG Number;
	ULONG MaxMhz;
	ULONG CurrentMhz;
	ULONG MhzLimit;
	ULONG MaxIdleState;
	ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

#pragma comment(lib, "Powrprof.lib")

int main()
{
	NTSTATUS status;
	LPBYTE pBuffer;

	SYSTEM_POWER_INFORMATION spi;
	pBuffer = new BYTE[sizeof(spi)];

	// Get the system power information
	status = ::CallNtPowerInformation(SystemPowerInformation, NULL, 0, pBuffer, sizeof(spi));
	if (status == STATUS_SUCCESS)
	{
		PSYSTEM_POWER_INFORMATION pspi = (PSYSTEM_POWER_INFORMATION)pBuffer;
		std::cout << "Max Idleness Allowed: " << pspi->MaxIdlenessAllowed << "\n"
			"Idleness: " << pspi->Idleness << "\n"
			"Time Remaining: " << pspi->TimeRemaining << "\n"
			"Cooling Mode: " << (int)pspi->CoolingMode << "\n"
			<< std::endl;
	}

	// Clean the buffer
	delete(pBuffer);

	// Get the power information of each installed processor
	std::this_thread::sleep_for(std::chrono::seconds(3));

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	PROCESSOR_POWER_INFORMATION ppi;
	pBuffer = new BYTE[sizeof(ppi) * si.dwNumberOfProcessors];

	status = ::CallNtPowerInformation(ProcessorInformation, NULL, 0, pBuffer, sizeof(ppi) * si.dwNumberOfProcessors);
	if (status == STATUS_SUCCESS)
	{
		PPROCESSOR_POWER_INFORMATION pppi = (PPROCESSOR_POWER_INFORMATION)pBuffer;
		for (size_t i = 0; i < si.dwNumberOfProcessors; i++)
		{
			std::cout << "Power levels for Processor #" << pppi->Number << "\n"
				"MaxMhz: " << pppi->MaxMhz << "\n"
				"CurrentMhz: " << pppi->CurrentMhz << "\n"
				"MhzLimit: " << pppi->MhzLimit << "\n"
				"MaxIdleState: " << pppi->MaxIdleState << "\n"
				"CurrentIdleState: " << pppi->CurrentIdleState << "\n"
				<< std::endl;

			pppi++;
		}
	}

	// Clean the buffer
	delete(pBuffer);

	system("pause");
	return 0;
}