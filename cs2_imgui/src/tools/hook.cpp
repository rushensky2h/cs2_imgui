#include "pch.h"
#include "hook.h"

bool hook_data::init(FARPROC func_in, FARPROC func_hook_in)
{
    if (is_hooked_)
    {
        if (func_ == func_in)
        {
            if (hook_ != func_hook_in)
            {
                hook_ = func_hook_in;
                hook();
                return true;
            }
        }

        clear();
    }

    func_ = func_in;
    hook_ = func_hook_in;

    return true;
}

void hook_data::hook(bool force)
{
    if ((!force && is_hooked_) || !func_)
        return;

    MH_CreateHook(func_, hook_, (void **)&m_orig_);
    MH_EnableHook(func_);

    is_hooked_ = true;
}

void hook_data::clear()
{
    if (!is_hooked_ || !func_)
        return;

    MH_DisableHook(func_);
    MH_RemoveHook(func_);

    is_hooked_ = false;
}

