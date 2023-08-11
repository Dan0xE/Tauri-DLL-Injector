#include <windows.h>
#include <stdbool.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdint.h>

bool DoesFileExist(const char *name)
{
	FILE *file = fopen(name, "r");
	if (file)
	{
		fclose(file);
		return true;
	}
	return false;
}

DWORD GetProcessIdFunc(const char *ProcessName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	if (Process32First(hSnap, &pe32))
	{
		do
		{
			if (strcmp(pe32.szExeFile, ProcessName) == 0)
			{
				CloseHandle(hSnap);
				return pe32.th32ProcessID;
			}
		} while (Process32Next(hSnap, &pe32));
	}
	CloseHandle(hSnap);
	return 0;
}

uintptr_t GetModuleBaseAddress(DWORD pid, const char *modName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (strcmp(modEntry.szModule, modName) == 0)
				{
					CloseHandle(hSnap);
					return (uintptr_t)modEntry.modBaseAddr;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
		CloseHandle(hSnap);
	}
	return 0;
}

bool LoadLibraryInject(DWORD ProcessId, const char *Dll)
{
	char CustomDLL[MAX_PATH];
	GetFullPathName(Dll, MAX_PATH, CustomDLL, 0);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(CustomDLL), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (!WriteProcessMemory(hProcess, allocatedMem, CustomDLL, sizeof(CustomDLL), NULL))
	{
		CloseHandle(hProcess);
		return false;
	}

	CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, allocatedMem, 0, 0);

	CloseHandle(hProcess);
	return true;
}

LPVOID getNTOpenFile()
{
	return GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");
}

LPVOID NTOpenFile = NULL;

bool ExecuteBypass(HANDLE hProcess)
{
	if (!NTOpenFile)
	{
		NTOpenFile = getNTOpenFile();
	}
	if (NTOpenFile)
	{
		char originalBytes[5];
		memcpy(originalBytes, NTOpenFile, 5);
		if (WriteProcessMemory(hProcess, NTOpenFile, originalBytes, 5, NULL))
		{
			return true;
		}
	}
	return false;
}

bool Backup(HANDLE hProcess)
{
	if (!NTOpenFile)
	{
		NTOpenFile = getNTOpenFile();
	}
	if (NTOpenFile)
	{
		char Orig[5];
		memcpy(Orig, NTOpenFile, 5);
		WriteProcessMemory(hProcess, NTOpenFile, Orig, 0, NULL);
		return true;
	}
	return false;
}

__declspec(dllexport) bool InjectDLL(int ProcessId, const char *DllPath)
{
	if (!DoesFileExist(DllPath))
	{
		return false;
	}
	return LoadLibraryInject((DWORD)ProcessId, DllPath);
}