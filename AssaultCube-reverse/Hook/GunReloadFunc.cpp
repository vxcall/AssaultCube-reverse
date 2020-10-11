#include "GunReloadFunc.h"
#include <iostream>
#include <Windows.h>
#include "detours.h"

#define baseOffset 0x0010F4F4
#define subgunOffset 0x374

using tReload = char (__thiscall*) (int this1, char a2);
tReload oReload = nullptr;

char __fastcall hReload(int this1, void* edx, char a2) {
    static int16_t MaxAmmo = *(int16_t*)((*(uintptr_t*)((uintptr_t)this1 + 0xC)) + 0x118);
    if (**(uintptr_t**)((uintptr_t)this1 + 0x10) != 0 && **(uintptr_t**)((uintptr_t)this1 + 0x14) != MaxAmmo) {
        std::cout << "Reloaded" << std::endl;
    }
    return oReload(this1, a2);
}

void hookReload() {
    uintptr_t Modulebase = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

    auto subgunVtablePtr = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(Modulebase + baseOffset) + subgunOffset);
    void** vTable = *reinterpret_cast<void***>(subgunVtablePtr);
    oReload = reinterpret_cast<tReload>(DetourFunction(reinterpret_cast<PBYTE>(vTable[7]), reinterpret_cast<PBYTE>(hReload)));
}