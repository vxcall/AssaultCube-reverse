#include "hookWrapper.h"
#include <iostream>

char __fastcall hooks::hkReload ( const int ecx, void* edx, const char a2 )
{
    static auto og = subGunHook.getOriginal< orgFuncs::reload_t > ( indices::reload ); // Use the getOriginal getter and receive it once at hook's beginning.

    static int16_t maxAmmo = *reinterpret_cast< int16_t* > ( *reinterpret_cast< uintptr_t* > ( static_cast< uintptr_t > ( ecx ) + 0xC ) + 0x118 );

    if ( **reinterpret_cast< uintptr_t** > ( static_cast< uintptr_t > ( ecx ) + 0x10 ) != 0 && **reinterpret_cast<
            uintptr_t** > ( static_cast< uintptr_t > ( ecx ) + 0x14 ) != maxAmmo )
    {
        std::cout << "Reloaded\n";
    }
    return og ( ecx, a2 ); // Let game do its thing
}