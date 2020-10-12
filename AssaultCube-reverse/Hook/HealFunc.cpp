#include "HealFunc.h"
#include <Windows.h>
#include <iostream>
#include "detours.h"

using tHeal = int (__stdcall*) (void* result, int a2);
tHeal oHeal = nullptr;

int __stdcall hHeal(void* result, int a2) { //result@<eax>, a2@<ecx>
    std::cout << "healed" << std::endl;
    return oHeal(result, a2);
}

__declspec(naked) void __stdcall func_hook(void* result, int a2) {
    __asm{
    push ebp
    mov ebp, esp
    push dword ptr[a2]
    push dword ptr[result]
    call hHeal
    retn 04h
    }
}

void hookHeal() {
    oHeal = reinterpret_cast<tHeal>(DetourFunction(reinterpret_cast<PBYTE>(0x0044672F), reinterpret_cast<PBYTE>(func_hook)));
}