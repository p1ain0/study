#include <stdio.h>
#include "windows.h"

#define SER_NAME  L"TEST"

int main(int argv, char **args)
{
	SC_HANDLE hSCM = NULL;
	SC_HANDLE hSer = NULL;
	
	if (argv != 2)
		return 0;
	wchar_t sys_path[1024];
	MultiByteToWideChar(
		CP_ACP, 
		0, 
		args[1],
		(int)strlen(args[1]) + 1, 
		sys_path, 
		1024);

	
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	
	if (hSCM == NULL)
	{
		goto faile;
	}
	printf("OpenSCManager Success!!!\n");

	hSer = CreateService(
		hSCM, 
		SER_NAME, 
		SER_NAME, 
		SERVICE_ALL_ACCESS, 
		SERVICE_KERNEL_DRIVER, 
		SERVICE_DEMAND_START, 
		SERVICE_ERROR_IGNORE, 
		sys_path,
		NULL, 
		NULL, 
		NULL, 
		NULL, 
		NULL);
	
	if (hSer == NULL)
	{
		DWORD dwError = GetLastError();
		if (dwError == ERROR_SERVICE_EXISTS)
		{
			hSer = OpenService(hSCM, SER_NAME, SERVICE_ALL_ACCESS);
			if (hSer == NULL)
				goto faile;
		}
		else
			goto faile;
	}

	printf("CreateService or OpenService success \n");

	getchar();

	BOOL bSucc = StartService(hSer, 0, NULL);
	printf("StartService:%u\n", bSucc);

	getchar();

	SERVICE_STATUS SerStatus = { 0 };
	bSucc = ControlService(hSer, SERVICE_CONTROL_STOP, &SerStatus);
	printf("StopService:%u\n", bSucc);
	
	getchar();
	DeleteService(hSer);


faile:
	if (hSCM != NULL)
	{
		CloseServiceHandle(hSCM);
		hSCM = NULL;
	}
	if (hSer != NULL)
	{
		CloseServiceHandle(hSer);
		hSer = NULL;
	}
	return 0;
}
