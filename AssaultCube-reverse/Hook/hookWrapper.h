#ifndef HOOKWRAPPER_H
#define HOOKWRAPPER_H

#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "MinHook.h"

class hook_t
{
public:
    hook_t ( ) = default;

    hook_t ( void* class_ptr ) : m_ptr ( class_ptr )
    {
    }

    template < typename t >
    void hook ( const uint32_t index, t fn_to_hook )
    {
        const auto vf_table = *static_cast< int32_t** > ( m_ptr );
        const auto org = reinterpret_cast< void* > ( vf_table[ index ] );

        m_hooked_functions[ index ] = nullptr;

        const auto create_hook_status = MH_CreateHook ( org, fn_to_hook, static_cast< void** > ( &m_hooked_functions[ index ] ) );
        const auto enable_hook_status = MH_EnableHook ( org );

        if ( create_hook_status != MH_OK )
            throw std::runtime_error ( "failed to create hook" );

        if ( enable_hook_status != MH_OK )
            throw std::runtime_error ( "failed to enable hook" );

        m_original_function.push_back ( org );
    }

    bool is_valid ( ) const
    {
        return !!m_ptr;
    }

    void unhook ( )
    {
        if ( !is_valid ( ) )
            return;

        for ( auto& org : m_original_function )
            MH_DisableHook ( org );
    }

    template < typename t >
    t get_original ( const int32_t index ) const
    {
        return reinterpret_cast< t > ( m_hooked_functions.at ( index ) );
    }

private:
    void* m_ptr;
    std::unordered_map< int, void* > m_hooked_functions;
    std::vector< void* > m_original_function;
};

#endif //HOOKWRAPPER_H
