#pragma once

#include "framework.h"

#include "tools/hook.h"

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
