#pragma once


#include "third_party/minhook/MinHook.h"
#pragma comment(lib, "third_party/minhook/libMinHook.x64.lib")

template <typename T = PVOID>
inline constexpr T get_v_func(PVOID func_in, int index)
{
	return (*static_cast<T **>(func_in))[index];
}

class hook_data
{
public:
	bool		init(FARPROC func_in, FARPROC func_hook_in);
	void		hook(bool force = false);
	void		clear();
	auto		is_hooked() const { return is_hooked_; }

	FARPROC		m_orig_;

private:
	FARPROC		func_{};
	FARPROC		hook_{};
	bool		is_hooked_{};
};
