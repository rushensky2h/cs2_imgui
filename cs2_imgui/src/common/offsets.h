#pragma once

#include "framework.h"

#include "tools/module_management.h"

enum module_list
{
	rendersystemdx11,
	client,
	max_count,
};

inline module_data modules[module_list::max_count];

using present_t = HRESULT(WINAPI *)(IDXGISwapChain*, UINT, UINT);
using resize_buffers_t = HRESULT(WINAPI *)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);

class CSwapChainDx11
{
public:
	char                __pad[0x178];
	IDXGISwapChain		*pSwapChain;
};

inline CSwapChainDx11* g_swapchain{};
inline HWND g_hwnd{};

class offsets
{
protected:
	offsets() = default;
	offsets(const offsets&) = delete;
	offsets &operator=(const offsets&) = delete;

public:
	static offsets &get()
	{
		static offsets instance;
		return instance;
	}

	bool find_modules();
	bool find_interfaces();
	bool find_offsets();

private:
	PVOID swapchain();
};