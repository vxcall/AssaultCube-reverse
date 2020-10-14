#ifndef GUNRELOADFUNC_H
#define GUNRELOADFUNC_H

#include "hookWrapper.h"

char __fastcall hReload(int ecx, void* edx, char a2);

namespace hooks
{
    namespace indices
    {
        constexpr int32_t reload = 7;
    }

    namespace org_functions
    {
        using reload_t = char (__thiscall*) (int, char);
    }
    hook_t reload_hook;
    void initialize()
    {
        uintptr_t Modulebase = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
        reload_hook = hook_t((void*)(*(uintptr_t *)(*(uintptr_t*)(Modulebase + 0x0010F4F4) + 0x374))/*hook class*/);
        reload_hook.hook(indices::reload, &hReload);
    }
    void uninitialize()
    {
        reload_hook.unhook();
    }
}

void hookReload();

#endif //GUNRELOADFUNC_H
