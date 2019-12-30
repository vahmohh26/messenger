#pragma once

#include <string>

#define LOG(MESSAGE) server::log::log(MESSAGE, __FILE__, __FUNCTION__, __LINE__)

namespace server::log
{
	using namespace std;

	void log(const string& message, const string& file, const string& function, int line);
}