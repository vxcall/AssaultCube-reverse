#include <Windows.h>
#include "Hook/hookWrapper.h"


void Detach() {
    hooks::uninitialize();
    fclose(stdout);
    fclose(stderr);
    FreeConsole();
}

DWORD WINAPI fMain(LPVOID lpParameter) {
    AllocConsole();
    FILE* fp = NULL;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    hooks::initialize();

    while(true) {
        if (GetAsyncKeyState(VK_DELETE) & 1) {
            break;
        }

        Sleep(10);
    }
    Detach();
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
    }
    return TRUE;
}