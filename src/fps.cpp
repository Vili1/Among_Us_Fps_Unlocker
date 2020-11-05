#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>

DWORD dwGetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID)
{
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32))
	{
		do 
		{
			if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
			{
				dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32));
	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}

int main()
{
	HWND hGameWindow;
	DWORD pID;
	HANDLE pHandle;
	char moduleName[] = "UnityPlayer.dll";
	DWORD baseAddress;
	DWORD AdressValue;
	int NewAdressValue = 0;

	std::cout << "External FPS Unlocker" << std::endl;
	std::cout << "VSync must be enabled!" << std::endl;
	while (true)
	{
		hGameWindow = FindWindow(NULL, "Among Us");
		GetWindowThreadProcessId(hGameWindow, &pID);
		pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
		DWORD clientBase = dwGetModuleBaseAddress(_T(moduleName), pID);
		ReadProcessMemory(pHandle, (LPCVOID)(clientBase + 0x0129B6B8), &baseAddress, sizeof(baseAddress), NULL);
		AdressValue = baseAddress + 0x140;
		WriteProcessMemory(pHandle, (LPVOID)(AdressValue), &NewAdressValue, sizeof(NewAdressValue), 0);
		CloseHandle(pHandle);
		Sleep(1000);
	}
}