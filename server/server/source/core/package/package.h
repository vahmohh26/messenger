#pragma once

#include <string>
#include <vector>

namespace server::core
{
	using namespace std;

	class package
	{
	public:
		package();
		package(const string& ip, const vector<char>& buffer);
		~package();

		void set_ip(const string& ip);
		void set_buffer(const vector<char>& buffer);

		const string& get_ip() const;
		const vector<char>& get_buffer() const;

	private:
		string _ip;
		vector<char> _buffer;
	};
}