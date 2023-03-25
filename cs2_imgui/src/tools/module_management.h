#pragma once

struct module_data
{
	HMODULE		handle;
	uintptr_t	base;
	uintptr_t	end;
	size_t		size;
};

class module_managament
{
protected:
	module_managament() = default;
	module_managament(const module_managament&) = delete;
	module_managament &operator=(const module_managament&) = delete;

public:
	static module_managament &get()
	{
		static module_managament instance;
		return instance;
	}

	uintptr_t	find_pattern(const char *signature, uintptr_t start, uintptr_t end, int offset = 0);

	uintptr_t	absolute(uintptr_t ptr);
	bool		find_module_by_name(const char* name, module_data* module);
	bool		address_not_in_space(uintptr_t ptr, uintptr_t low, uintptr_t high);

};