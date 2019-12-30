#include "log.h"

#include <iostream>

namespace server::log
{
	void log(const string& message, const string& file, const string& function, int line)
	{
		cout << "message: " << message << endl;
		cout << "file: " << file << endl;
		cout << "function: " << function << endl;
		cout << "line: " << line << endl;
	}
}