#pragma once

#include <queue>

#include "../package/package.h"

namespace server::core
{
	using namespace std;

	queue<package> login(const package& request_package);
	queue<package> logout(const package& request_package);
}