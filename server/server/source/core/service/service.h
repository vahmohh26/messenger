#pragma once

#include <queue>

#include <protocol/package/package.h>

namespace server::core
{
	using namespace std;

	queue<protocol::package> login(const protocol::package& request_package);
	queue<protocol::package> logout(const protocol::package& request_package);
}