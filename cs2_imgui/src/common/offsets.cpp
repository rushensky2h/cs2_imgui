#include "pch.h"
#include "offsets.h"

bool offsets::find_modules()
{
    if (!module_managament::get().find_module_by_name("rendersystemdx11.dll", &modules[module_list::rendersystemdx11]))
    {
        console::get().print("module rendersystemdx11.dll not found.\n");
        return false;
    }

    if (!module_managament::get().find_module_by_name("client.dll", &modules[module_list::client]))
    {
        console::get().print("module client.dll not found.\n");
        return false;
    }

    return true;
}

bool offsets::find_interfaces()
{
    return true;
}

bool offsets::find_offsets()
{
    g_swapchain = **reinterpret_cast<CSwapChainDx11 ***>(swapchain());

    BYTE *ptr_hwnd = *(PBYTE *)module_managament::get().absolute(module_managament::get().find_pattern("48 89 3D ? ? ? ? FF 15 ? ? ? ? 89 3D ? ? ? ?",
        modules[module_list::rendersystemdx11].base, modules[module_list::rendersystemdx11].end, 0x3));

    g_hwnd = *(HWND *)(ptr_hwnd + 0x250);

    return true;
}

PVOID offsets::swapchain()
{
    auto ptr = module_managament::get().absolute(
        module_managament::get().find_pattern("66 0F 7F 05 ? ? ? ? 66 0F 7F 0D ? ? ? ? 48 89 35 ? ? ? ?",
            modules[module_list::rendersystemdx11].base, modules[module_list::rendersystemdx11].end, 0x4));

    if (module_managament::get().address_not_in_space(ptr, modules[module_list::rendersystemdx11].base, modules[module_list::rendersystemdx11].end))
    {
        console::get().print("%s not found.\n", __FUNCTION__);
        return NULL;
    }

    return (PVOID)ptr;
}