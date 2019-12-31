#pragma once

#include <core/core.h>

namespace server
{
	extern bool global_stop;
	extern core::core global_core;

	int execute(int argc, char* argv[]);
}