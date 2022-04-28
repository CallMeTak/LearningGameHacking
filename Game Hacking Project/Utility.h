#pragma once
#include <cstdint>
#include <Windows.h>
#include <string>
#include <vector>
class Utility
{
public:
	static DWORD64 RPM64(HANDLE hProcess, DWORD64 offset);
	static int RPMInt(HANDLE hProcess, uintptr_t addr);
	static std::string RPMString(HANDLE hProcess, DWORD64 offset);

	uintptr_t FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets);
	static uintptr_t GetProcId(const wchar_t *procName);
	static uintptr_t GetModuleBase(DWORD procID, const wchar_t* modName);
};

