#pragma once

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "tools/console.h"

#include "third_party/imgui/imgui.h"
#include "third_party/imgui/imgui_internal.h"
#include "third_party/imgui/backends/imgui_impl_dx11.h"
#include "third_party/imgui/backends/imgui_impl_win32.h"
