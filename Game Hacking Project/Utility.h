#pragma once
#include "pch.h"
#include "GameObjects.h"

constexpr  float PI = 3.1415927f;

class Utility
{
public:
	template <typename T>
    static T RPM(HANDLE hProcess, uintptr_t addr){
	    T buffer;
		ReadProcessMemory(hProcess, (BYTE*)addr, &buffer, sizeof(T), NULL);
		return buffer;
	}
	static std::string RPMString(HANDLE hProcess, DWORD64 offset);

	uintptr_t FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets);
	static uintptr_t GetProcId(const wchar_t *procName);
	static uintptr_t GetModuleBase(DWORD procID, const wchar_t* modName);

	static Vec3 CalcAngle(Vec3& angle1, Vec3& angle2);
	static Ent* GetBestTarget(EntList* list, unsigned int numPlayers, Vec3 localCoords);
	static bool isValidEnt(Ent* ent);
};