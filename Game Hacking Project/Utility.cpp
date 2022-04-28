#include "Utility.h"
#include <TlHelp32.h>
#include <iostream>
DWORD64 Utility::RPM64(HANDLE hProcess, DWORD64 offset)
{
    DWORD64 buffer;
    ReadProcessMemory(hProcess, (LPVOID)offset, &buffer, sizeof(float), NULL);
    return buffer;
}

int Utility::RPMInt(HANDLE hProcess, uintptr_t addr)
{
    int buffer;
    ReadProcessMemory(hProcess, (BYTE*)addr, &buffer, sizeof(int), NULL);
    return buffer;
}

std::string Utility::RPMString(HANDLE hProcess, DWORD64 offset)
{
    std::string buffer;
    ReadProcessMemory(hProcess, (LPCVOID)offset, &buffer, sizeof(DWORD64), NULL);
    return buffer;
}

uintptr_t Utility::FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(hProcess, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}

uintptr_t Utility::GetProcId(const wchar_t *procName)
{
    DWORD procID = 0;
    HANDLE hSnap;
    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, procID);
    PROCESSENTRY32W procEntry{};

    // Checks if handle in valid
    if (hSnap != INVALID_HANDLE_VALUE) {
        procEntry.dwSize = sizeof(procEntry);

        // If can loop through modules
        if (Process32FirstW(hSnap, &procEntry)) {
            do {
                // If this is the module we want, then set baseAddr
                if (_wcsicmp(procEntry.szExeFile, procName) == 0) {
                    CloseHandle(hSnap);
                    std::cout << "Found process: " << procName << '\n';
                    procID = procEntry.th32ProcessID;
                    return procID;
                }
            }
            // If not, continue loop to next module
            while (Process32NextW(hSnap, &procEntry));

        }
        // Else if can't loop for some reason
        else {
            std::cout << "Cannot loop through process list. Error: " << GetLastError() << '\n';
        }
    }
    // Else if handle is invalid
    else {
        std::cout << "Error obtaining procID. Handle invalif. Error: " << GetLastError() << '\n';
    }
    CloseHandle(hSnap);
    return 0;
}

uintptr_t Utility::GetModuleBase(DWORD procID, const wchar_t* modName)
{
    HANDLE modHandle;
    uintptr_t baseAddr = 0;
    modHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, procID);
    MODULEENTRY32W mEntry{};

    // Checks if handle in valid
    if (modHandle != INVALID_HANDLE_VALUE) {
        mEntry.dwSize = sizeof(mEntry);

        // If can loop through modules
        if (Module32FirstW(modHandle, &mEntry)) {
            do {
                // If this is the module we want, then set baseAddr
                if (_wcsicmp(mEntry.szModule, modName) == 0) {
                    CloseHandle(modHandle);
                    std::cout << "Found module: " << modName << '\n';
                    baseAddr = (uintptr_t)mEntry.modBaseAddr;
                    return baseAddr;
                }
            }
            // If not, continue loop to next module
            while (Module32NextW(modHandle, &mEntry));

        }
        // Else if can't loop for some reason
        else {
            std::cout << "Cannot loop through module list. Error: " << GetLastError() << '\n';
        }
    }
    // Else if mod handle is invalid
    else {
        std::cout << "Error obtaining module base address. Cannot obtain module handle. Error: " << GetLastError() << '\n'
                  << "Base address: " << baseAddr << '\n';
    }
    CloseHandle(modHandle);
    return 0;
}
