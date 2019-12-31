#pragma once

#include <string>

#define LOG() server::log::log(__FILE__, __FUNCTION__, __LINE__)
#define LOG_MESSAGE(MESSAGE) server::log::log(__FILE__, __FUNCTION__, __LINE__, MESSAGE)

namespace server::log
{
	using namespace std;

	void log(const string& file, const string& function, int line, const string& message = "");
}