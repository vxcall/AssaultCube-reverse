#include "HealFunc.h"
#include <Windows.h>
#include <iostream>
#include "detours.h"

using tHeal = void* (__stdcall*) (void*, int);
tHeal oHeal = nullptr;

void* __stdcall hHeal(void* player, int a2) { //result@<eax>, a2@<ecx>
    std::cout << "healed" << std::endl;
    return oHeal(player, a2);
}

__declspec(naked) void __stdcall func_hook(void* player, int a2) 
{
  __asm
  {
    push ebp
    mov ebp, esp
    push dword ptr[a2]
    push dword ptr[player]
    call hHeal
    retn 04h
  }

void hookHeal() {
    oHeal = reinterpret_cast<tHeal>(DetourFunction(reinterpret_cast<PBYTE>(0x0044672F), reinterpret_cast<PBYTE>(func_hook)));
}
