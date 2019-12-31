#include "log.h"

#include <iostream>

namespace server::log
{
	void log(const string& file, const string& function, int line, const string& message)
	{
		cout << "file: " << file << endl;
		cout << "function: " << function << endl;
		cout << "line: " << line << endl;

		if (!message.empty())
		{
			cout << "message: " << message << endl;
		}
	}
}