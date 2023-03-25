#include "pch.h"
#include "module_management.h"

#include <vector>

std::vector<int> pattern_to_byte(const char *pattern)
{
	auto nBytes = std::vector<int>{ };
	auto start = const_cast<char *>(pattern);
	auto end = const_cast<char *>(pattern) + strlen(pattern);

	for (auto current = start; current < end; ++current)
	{
		if (*current == '?')
		{
			++current;
			if (*current == '?')
				++current;

			nBytes.push_back(-1);
		}
		else
			nBytes.push_back(strtoul(current, &current, 16));
	}

	return nBytes;
};

uintptr_t module_managament::find_pattern(const char *signature, uintptr_t start, uintptr_t end, int offset)
{
	if (start && end)
	{
		const auto pattern = pattern_to_byte(signature);
		const auto patternBytes = pattern.data();
		const size_t patternLength = pattern.size();

		bool up = false;

		if (end < start)
			up = true;

		if (!up)
		{
			for (auto i = start; i < end - patternLength; ++i)
			{
				bool found = true;

				for (auto j = 0u; j < patternLength; ++j)
				{
					if (patternBytes[j] != -1 && (CHAR)patternBytes[j] != *(PCHAR)(i + j))
					{
						found = false;
						break;
					}
				}

				if (found)
					return i + offset;
			}
		}
		else
		{
			for (auto i = start; i > end - patternLength; --i)
			{
				bool found = true;

				for (auto j = 0u; j < patternLength; ++j)
				{
					if (patternBytes[j] != -1 && (CHAR)patternBytes[j] != *(PCHAR)(i + j))
					{
						found = false;
						break;
					}
				}

				if (found)
					return i + offset;
			}
		}
	}

	return 0;
}

uintptr_t module_managament::absolute(uintptr_t ptr)
{
	return ptr + *(PDWORD)ptr + 0x4;
}

bool module_managament::find_module_by_name(const char *name, module_data *module)
{
	if (!name || !strlen(name))
		return false;

	HMODULE hModuleDll = GetModuleHandleA(name);

	if (!hModuleDll)
		return false;

	if (hModuleDll == INVALID_HANDLE_VALUE)
		return false;

	MEMORY_BASIC_INFORMATION mem;

	if (!VirtualQuery(hModuleDll, &mem, sizeof(mem)))
		return false;

	if (mem.State != MEM_COMMIT)
		return false;

	if (!mem.AllocationBase)
		return false;

	IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)mem.AllocationBase;
	IMAGE_NT_HEADERS *pe = (IMAGE_NT_HEADERS *)((uintptr_t)dos + (uintptr_t)dos->e_lfanew);

	if (pe->Signature != IMAGE_NT_SIGNATURE)
		return false;

	module->handle = hModuleDll;
	module->base = (uintptr_t)(mem.AllocationBase);
	module->size = (uintptr_t)(pe->OptionalHeader.SizeOfImage);
	module->end = module->base + module->size - 1;

	return true;
}

bool module_managament::address_not_in_space(uintptr_t ptr, uintptr_t low, uintptr_t high)
{
	if (low > high)
	{
		auto reverse = high;
		high = low;
		low = reverse;
	}

	return ((ptr < low) || (ptr > high));
}
