#include <Windows.h>
#include <iostream>

#define PrintHex(x) std::cout << "0x" << std::hex << x << std::endl

#define baseOffset 0x0010F4F4
#define subgunOffset 0x374

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

    uintptr_t Modulebase = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
    auto* subgunVtablePtr = reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(Modulebase + baseOffset) + subgunOffset));

    PrintHex(*subgunVtablePtr + 0x0C);

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