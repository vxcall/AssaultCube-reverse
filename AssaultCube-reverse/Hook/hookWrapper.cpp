#include "hookWrapper.h"
#include "MinHook.h"
#include "hook_t.h"
#include "../offsets.h"
#include <iostream>

#define REGISTER_HOOK(hook_in, index_n, hook_function) hook_in.hook(index_n, hook_function);\
std::cout << "hooked " << #hook_function << std::endl

namespace hooks {
    hook_t subGunHook;

    void initialize() {
        try {
            if (MH_Initialize() != MH_OK) // Always remember to initialize minhook at the beginning of your hook initializing ( you forgot this )
                throw std::exception("MH_Initialize failed!");

            const uintptr_t modBase = reinterpret_cast< uintptr_t > ( GetModuleHandle(nullptr));

            subGunHook = hook_t /* vtable pointer = class ptr, do this because otherwise u r just reusing the same vtable over and over again when doing e.g reload and shoot hook, this allows u to do what's done below. */
                    (
                            reinterpret_cast< void * > ( *reinterpret_cast< uintptr_t * > (
                                    *reinterpret_cast< uintptr_t * >
                                    ( modBase + offsets::localPlayer ) + offsets::secondaryGun ))
                    );
        }
        catch (const std::exception &ex) {
            MessageBoxA(nullptr, ex.what(), "Error", 0);
        }

        REGISTER_HOOK (subGunHook, indices::reload, hkReload); // A nice little macro :)
        REGISTER_HOOK (subGunHook, indices::shoot, hkShoot);
    }

    void uninitialize() {
        subGunHook.unhook();
        MH_Uninitialize(); // Always remember to call minhook
    }
}