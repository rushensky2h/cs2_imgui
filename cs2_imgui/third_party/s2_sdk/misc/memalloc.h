#pragma once

#include "../sdk.h"

class IMemAlloc
{
public:
    void *Alloc(std::size_t size) 
    {
        typedef void*(__thiscall *CallVFunctionFn)(void *, std::size_t);
        return get_v_func<CallVFunctionFn>(this, 1)(this, size);
    }

    void *ReAlloc(const void *p, std::size_t size)
    {
        typedef void *(__thiscall *CallVFunctionFn)(void *, const void *, std::size_t);
        return get_v_func<CallVFunctionFn>(this, 3)(this, p, size);
    }

    void Free(const void *p) 
    {
        typedef void(__thiscall *CallVFunctionFn)(void *, const void *);
        return get_v_func<CallVFunctionFn>(this, 5)(this, p);
    }

    std::size_t GetSize(const void *p)
    {
        typedef std::size_t(__thiscall *CallVFunctionFn)(void *, const void *);
        return get_v_func<CallVFunctionFn>(this, 21)(this, p);
    }
};
inline IMemAlloc *g_memalloc{};
