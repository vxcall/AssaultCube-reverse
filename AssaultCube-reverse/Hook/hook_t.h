#pragma once

#include <unordered_map>
#include <vector>
#include "MinHook.h"

//HOOK_T.H
class hook_t
{
public:
    hook_t() = default;

    explicit hook_t(void* classPtr) : m_Ptr(classPtr) {}

    template<typename t>
    void hook(const uint32_t index, t fnToHook)
    {
        int32_t* const vTable = *static_cast<int32_t**>(m_Ptr);
        void* const org = reinterpret_cast<void*>(vTable[index]);

        m_hookedFunctions[index] = nullptr;

        try
        {
            const MH_STATUS createHookStatus = MH_CreateHook(org, fnToHook, static_cast<void**>(&m_hookedFunctions[index]));
            const MH_STATUS enableHookStatus = MH_EnableHook(org);

            if (createHookStatus != MH_OK)
                throw std::exception("MH_CreateHook failed!");

            if(enableHookStatus != MH_OK)
                throw std::exception("MH_EnableHook failed!");
        }
        catch(const std::exception& ex)
        {
            MessageBoxA(nullptr, ex.what(), "Error", 0);
        }

        m_originalFunctions.push_back(org);
    }

    bool isValid() const
    {
        return !!m_Ptr;
    }

    void unhook()
    {
        if (!isValid())
            return;

        for (auto& org : m_originalFunctions)
            MH_DisableHook (org);
    }

    template<typename t>
    t getOriginal(const int32_t index) const
    {
        return reinterpret_cast<t>(m_hookedFunctions.at(index));
    }

private:
    void* m_Ptr;
    std::unordered_map<int, void*> m_hookedFunctions;
    std::vector<void*> m_originalFunctions;
};

