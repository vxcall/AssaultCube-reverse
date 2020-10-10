#include "GunReloadFunc.h"
#include <iostream>
#include <Windows.h>
#include "detours.h"

#define baseOffset 0x0010F4F4
#define subgunOffset 0x374

using tReload = char (__thiscall*) (int this1, char a2);
tReload oReload = nullptr;

char __fastcall hReload(int this1, void* edx, char a2) {
    std::cout << "Reloaded" << std::endl;
    return oReload(this1, a2);
}

void hookReload() {
    uintptr_t Modulebase = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

    auto subgunVtablePtr = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(Modulebase + baseOffset) + subgunOffset);
    void** vTable = *reinterpret_cast<void***>(subgunVtablePtr);
    oReload = reinterpret_cast<tReload>(DetourFunction(reinterpret_cast<PBYTE>(vTable[7]), reinterpret_cast<PBYTE>(hReload)));
}