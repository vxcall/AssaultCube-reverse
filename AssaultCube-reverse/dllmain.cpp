#include <Windows.h>
#include <iostream>
#include "detours.h"

#define PrintHex(x) std::cout << "0x" << std::hex << x << std::endl

#define baseOffset 0x0010F4F4
#define subgunOffset 0x374

void Detach() {
    fclose(stdout);
    fclose(stderr);
    FreeConsole();
}

using tShoot = char (__thiscall*) (DWORD* this1, int* a2);
tShoot oShoot = nullptr;

char __fastcall hShoot(DWORD* this1, int* a2)
{
    std::cout << "shoooooooooot" << std::endl;
    return oShoot(this1, a2);
}

uintptr_t Modulebase = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

void hookShoot()
{
    auto* subgunVtablePtr = reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(Modulebase + baseOffset) + subgunOffset));
    oShoot = reinterpret_cast<tShoot>(DetourFunction(reinterpret_cast<PBYTE>(*(subgunVtablePtr+3)), reinterpret_cast<PBYTE>(hShoot)));
}

DWORD WINAPI fMain(LPVOID lpParameter) {
    AllocConsole();
    FILE* fp = NULL;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    hookShoot();

   // PrintHex(*subgunVtablePtr + 0x0C);

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