// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "common/offsets.h"

#include "hooks.h"

HMODULE g_module{};

void main_thread()
{
    auto& console = console::get();
    console.attach();

    console.print("Starting thread...\n");

    auto& offsets = offsets::get();

    while (true)
    {
        if (offsets.find_modules() && offsets.find_interfaces())
            break;

        Sleep(100);
    }

    if (!offsets.find_offsets())
        return;

    hooks::get().init();

    while (true)
    {
        if (GetAsyncKeyState(VK_END))
            break;
    }

    console.print("Detach...\n");
    
    Sleep(1000);

    hooks::get().shutdown();

    console.detach();

    FreeLibraryAndExitThread(g_module, EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        g_module = hModule;

        CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(main_thread), NULL, NULL, NULL);
        return TRUE;
    }

    return FALSE;
}

