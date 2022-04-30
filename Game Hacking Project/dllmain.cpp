#include "pch.h"
#include "GameObjects.h"
#include "Offsets.h"

void listEnts();
bool isValidEnt(Ent* ent);

// Gets the base of the module
uintptr_t modBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

DWORD WINAPI MyThread(HMODULE hModule) {
    // Creates a new console
    AllocConsole();
    FILE* f;
    // Redirects output to this console
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "This is working. \n";

    std::cout << "modBase: " << std::hex << modBase << "\n";

    Ent* localPlayerPtr{ nullptr };
    bool unload = false;
    bool noRecoil = false;
    bool noClip = false;
    bool maxHealth = false;

    while (!unload) {
        // Sleep for a short time to avoid performance loss
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        // Cast the player pointer (modBase + 0x10f4f4) to a playerent object ptr
        localPlayerPtr = *(Ent**)(modBase + 0x10F4F4);

        // Checks if the pointer is valid. If not then do nothing.
        if (!localPlayerPtr) { continue; }


        if (noRecoil) { 
            localPlayerPtr->currWeapon->gunData->recoil1 = 0;
            localPlayerPtr->currWeapon->gunData->recoil2 = 0; 
        }

        // Enable noclip
        if (noClip) localPlayerPtr->playerMode = 4;
        // Else if disabled
        else {
            // If player is not dead (1), set playerMode back to 0 (alive).
            if (localPlayerPtr->playerMode != 1)
                localPlayerPtr->playerMode = 0; }

        if (maxHealth) { localPlayerPtr->health = 9999; }
        else {
            if (localPlayerPtr->health > 100) { localPlayerPtr->health = 100; }
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            std::cout << "Player address is 0x" << std::hex << localPlayerPtr << '\n';
            }

        // If numpad2 is pressed, then the cheat will be unloaded
        if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
            unload = true;
        }

        // No-clip
        if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
            noClip = !noClip;
            std::cout << "No-clip toggled.\n";
        }
        // noRecoil
        if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
            noRecoil = !noRecoil;
            std::cout << "No-recoil enabled.\n";
        }
        // maxHealthw
        if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
            maxHealth = !maxHealth;
            std::cout << "Max health toggled.\n";
        }
        // Print health of all entities
        if (GetAsyncKeyState(VK_NUMPAD6) & 1) {
            listEnts();
        }
    }
    // Closes the stream, exits the console, and exits the thread
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}
BOOL APIENTRY DllMain(
        HMODULE hModule,
        DWORD  ul_reason_for_call,
        LPVOID lpReserved)
    {
        switch (ul_reason_for_call) {
        // If this dll is attached to a process, then create a new thread that runs MyModule()
        case DLL_PROCESS_ATTACH:
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MyThread, hModule, 0, 0);
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
        }
        return TRUE;
    }


void listEnts() {
    // Create a pointer to the EntityList
    EntList* entList = *(EntList**)(modBase + EntityListOffset);

    unsigned int numPlayers = *(int*)(modBase + numPlayersOffset);

    std::cout << "Numplayers is: " << std::dec << numPlayers << '\n';

    // Loop through each player in entList
    for (unsigned int i = 0; i < numPlayers; i++) {
        // If this ent is valid, then print its name
        if (isValidEnt(entList->ents[i])) {
            std::cout << entList->ents[i]->name << '\n';
        }
    }

}

bool isValidEnt(Ent* ent) {
    if (ent) {
        // Checks location of vTable to determine is Ent is a pleyerent or botent

        if (ent->vTable == 0x4E4A98 || ent->vTable == 0x4E4AC0) {
            return true;
        }
    }
    return false;
}