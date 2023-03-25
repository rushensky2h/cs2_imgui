#pragma once

#include <chrono>
#include <iostream>
#include "framework.h"

class console
{
public:
    console() = default;
    console(const console&) = delete;
    console &operator=(const console&) = delete;

    static console &get()
    {
        static console instance;
        return instance;
    }

    void attach();
    void detach();
    void print(const char* fmt, ...);
        
private:
    bool attached_{};

    HANDLE out_{};
    HANDLE err_{};
    HANDLE in_{};
    HANDLE old_out_{};
    HANDLE old_err_{};
    HANDLE old_in_{};
};