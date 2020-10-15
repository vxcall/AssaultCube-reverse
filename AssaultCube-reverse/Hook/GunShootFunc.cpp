#include "GunShootFunc.h"
#include "hookWrapper.h"
#include <iostream>

char __fastcall hooks::hkShoot ( int* ecx, void* edx, const int a2, const int a3, const int a4 )
{
    static auto og = subGunHook.getOriginal< orgFuncs::shoot_t > ( indices::shoot ); // Use the getOriginal getter and receive it once at hook's beginning.
    std::cout << "ammo in magazine: " << **reinterpret_cast< uintptr_t** > ( reinterpret_cast< uintptr_t > ( ecx ) + 0x14 ) - 1 << std::endl;
    return og ( ecx, a2, a3, a4 ); // Let the game do its thing
}