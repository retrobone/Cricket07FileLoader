// Cricket 07 File Loader v1.0 (by retrobone) - 16.01.2026

#include <windows.h>
#include <string>
#include <algorithm>
#include <sys/stat.h>
#include <stdio.h>


constexpr uintptr_t HOOK_FILELOADER = 0x006E6B20;
constexpr int PROLOGUE_SIZE = 6;

typedef DWORD(__cdecl* ProcessAndHashFileName_t)(DWORD, char*, BYTE);
ProcessAndHashFileName_t Gateway_FileLoader = nullptr;


static bool FileExists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

// Bat name decoder for game, game stores bat id in a single byte in a roster
static int GetFixedBatID(const std::string& originalFilename) {
    size_t pos = originalFilename.find("bt");
    if (pos == std::string::npos) return 0;
    if (pos + 3 >= originalFilename.length()) return 0;

    char tensChar = originalFilename[pos + 2];
    char onesChar = originalFilename[pos + 3];

    bool tensBugged = (tensChar < '0' || tensChar > '9');
    bool onesBugged = (onesChar < '0' || onesChar > '9');

    if (!tensBugged && !onesBugged) return 0;

    int tens = tensChar - 48;
    int ones = onesChar - 48;
    int realID = (tens * 10) + ones;

    if (realID > 99 && realID < 300) {
        return realID;
    }
    return 0;
}

// Direct filename loader logic
static DWORD __cdecl Detour_ProcessAndHashFileName(DWORD param_1, char* fileName, BYTE useHash)
{
    if (fileName)
    {
        std::string rawPath = fileName;

        // Lowercase conversion
        std::string cleanPath = fileName;
        std::transform(cleanPath.begin(), cleanPath.end(), cleanPath.begin(), ::tolower);
        std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');

        // Check if direct file exists
        if (FileExists(cleanPath))
        {
            return Gateway_FileLoader(param_1, (char*)cleanPath.c_str(), 0);
        }

        // Bat name fix
        int fixedID = GetFixedBatID(rawPath);

        if (fixedID > 0)
        {
            char newPath[256];
            sprintf_s(newPath, "misc/textures/bt%d.fsh", fixedID);

            if (FileExists(newPath))
            {
                return Gateway_FileLoader(param_1, newPath, 0);
            }
        }
    }

    return Gateway_FileLoader(param_1, fileName, useHash);
}

// Injecting fileloader logic
static void InjectFileLoaderHook() {
    BYTE* trampoline = (BYTE*)VirtualAlloc(NULL, 15, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (!trampoline) {
        MessageBoxA(NULL, "Failed to allocate memory!", "Loader Error", MB_ICONERROR);
        return;
    }

    memcpy(trampoline, (void*)HOOK_FILELOADER, PROLOGUE_SIZE);

    trampoline[PROLOGUE_SIZE] = 0xE9;
    DWORD returnAddr = (HOOK_FILELOADER + PROLOGUE_SIZE) - ((DWORD)trampoline + PROLOGUE_SIZE) - 5;
    *(DWORD*)(trampoline + PROLOGUE_SIZE + 1) = returnAddr;

    Gateway_FileLoader = (ProcessAndHashFileName_t)trampoline;

    DWORD oldProtect;
    if (VirtualProtect((void*)HOOK_FILELOADER, PROLOGUE_SIZE, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        BYTE* patch = (BYTE*)HOOK_FILELOADER;
        patch[0] = 0xE9;
        *(DWORD*)(patch + 1) = (DWORD)((DWORD)Detour_ProcessAndHashFileName - HOOK_FILELOADER - 5);
        patch[5] = 0x90;
        VirtualProtect((void*)HOOK_FILELOADER, PROLOGUE_SIZE, oldProtect, &oldProtect);
    }
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        InjectFileLoaderHook();
    }
    return TRUE;
}
