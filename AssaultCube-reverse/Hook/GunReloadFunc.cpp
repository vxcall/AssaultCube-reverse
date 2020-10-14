#include <Windows.h>
#include "detours.h"
#include <iostream>

#define baseOffset 0x0010F4F4
#define subgunOffset 0x374

using tReload = char (__thiscall*) (int this1, char a2);
tReload oReload = nullptr;

//ecx and edx are passed to the function, others are puched onto stack
char __fastcall hReload(int ecx, void* edx, char a2) {
    static int16_t MaxAmmo = *(int16_t*)((*(uintptr_t*)((uintptr_t)ecx + 0xC)) + 0x118);
    if (**(uintptr_t**)((uintptr_t)ecx + 0x10) != 0 && **(uintptr_t**)((uintptr_t)ecx + 0x14) != MaxAmmo) {
        std::cout << "Reloaded" << std::endl;
    }
    return oReload(ecx, a2);
}

void hookReload() {
    uintptr_t Modulebase = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

    auto subgunVtablePtr = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(Modulebase + baseOffset) + subgunOffset);
    void** vTable = *reinterpret_cast<void***>(subgunVtablePtr);
    oReload = reinterpret_cast<tReload>(DetourFunction(reinterpret_cast<PBYTE>(vTable[7]), reinterpret_cast<PBYTE>(hReload)));
}