#include "GunShootFunc.h"
#include <Windows.h>
#include <iostream>
#include "detours.h"

#define baseOffset 0x0010F4F4
#define subgunOffset 0x374

using tShoot = char (__thiscall*) (int *this1, int a2, int a3, int a4);
tShoot oShoot = nullptr;

char __fastcall hShoot(int *this1, void* edx, int a2, int a3, int a4)
{
    std::cout << "ammo in magazine: " << **(uintptr_t**)((uintptr_t)this1 + 0x14) - 1 << std::endl;
    return oShoot(this1, a2, a3, a4);
}

void hookShoot()
{
    uintptr_t Modulebase = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

    void*** pSubgunVtable = *reinterpret_cast<void****>(*reinterpret_cast<uintptr_t*>(Modulebase + baseOffset) + subgunOffset);
    oShoot = reinterpret_cast<tShoot>(DetourFunction(reinterpret_cast<PBYTE>((*pSubgunVtable)[4]), reinterpret_cast<PBYTE>(hShoot)));
}