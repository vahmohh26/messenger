#pragma once

#include <queue>

#include "../session/session.h"
#include "../package/package.h"

namespace server::core
{
	using namespace std;

	queue<package> login(session& session, const package& request_package);
	queue<package> logout(session& session, const package& request_package);
	queue<package> message(session& session, const package& request_package);
}