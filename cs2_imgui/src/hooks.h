#pragma once

#include "framework.h"

#include "tools/hook.h"

using present_t = HRESULT(WINAPI *)(IDXGISwapChain *, UINT, UINT);


struct handle_data
{
	DWORD64 process_id;
	HWND	window_handle;
};

class hooks
{
protected:
	hooks() = default;
	hooks(const hooks&) = delete;
	hooks &operator=(const hooks&) = delete;

public:
	static hooks &get()
	{
		static hooks instance;
		return instance;
	}

	void init();
	void shutdown();
};