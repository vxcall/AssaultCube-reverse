#pragma once

#include "hook_t.h"

namespace hooks
{
    namespace orgFuncs
    {
        using reload_t = char(__thiscall*) (int, char);
        using shoot_t = char(__thiscall*) (void*, int, int, int);
    }

    namespace indices
    {
        static constexpr int reload = 7;
        static constexpr int shoot = 4;
    }

    void initialize();
    void uninitialize();

    extern hook_t subGunHook;

    char __fastcall hkReload(int ecx, void* edx, char a2);
    char __fastcall hkShoot(int* ecx, void* edx, int a2, int a3, int a4);
}
