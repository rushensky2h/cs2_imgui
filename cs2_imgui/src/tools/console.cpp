#include "pch.h"
#include "console.h"

void console::attach()
{
    old_out_ = GetStdHandle(STD_OUTPUT_HANDLE);
    old_err_ = GetStdHandle(STD_ERROR_HANDLE);
    old_in_ = GetStdHandle(STD_INPUT_HANDLE);

    if (AllocConsole() && AttachConsole(GetCurrentProcessId()))
    {
        attached_ = true;
    }

    out_ = GetStdHandle(STD_OUTPUT_HANDLE);
    err_ = GetStdHandle(STD_ERROR_HANDLE);
    in_ = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleMode(out_,
        ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

    SetConsoleMode(in_, 
        ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
        ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
}

void console::detach()
{
    if (out_ && err_ && in_)
    {
        FreeConsole();

        attached_ = false;

        if (old_out_)
            SetStdHandle(STD_OUTPUT_HANDLE, old_out_);
        if (old_err_)
            SetStdHandle(STD_ERROR_HANDLE, old_err_);
        if (old_in_)
            SetStdHandle(STD_INPUT_HANDLE, old_in_);
    }
}

void console::print(const char *fmt, ...)
{
    char buf[4096] = { 0 };

    va_list args;
    va_start(args, fmt);
    vsprintf_s(buf, fmt, args);
    va_end(args);

    WriteConsoleA(out_, buf, static_cast<DWORD>(strlen(buf)), nullptr, nullptr);
}
