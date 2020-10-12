#include <Windows.h>
#include <iostream>
#include "Hook/GunShootFunc.h"
#include "Hook/GunReloadFunc.h"
#include "Hook/HealFunc.h"

#define PrintHex(val) std::cout << "0x" << std::hex << val << std::endl

void Detach() {
    fclose(stdout);
    fclose(stderr);
    FreeConsole();
}

DWORD WINAPI fMain(LPVOID lpParameter) {
    AllocConsole();
    FILE* fp = NULL;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    hookShoot();
    hookReload();
    hookHeal();

    while(true) {
        if (GetAsyncKeyState(VK_DELETE) & 1) {
            break;
        }

        Sleep(10);
    }

    FreeLibraryAndExitThread(static_cast<HMODULE>(lpParameter), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        HANDLE hThread = CreateThread(nullptr, 0, fMain, hModule, 0, nullptr);
        if (hThread)
        {
            CloseHandle(hThread);
        }
    } else if (dwReason == DLL_PROCESS_DETACH && !lpReserved)
    {
        Detach();
    }
    return TRUE;
}