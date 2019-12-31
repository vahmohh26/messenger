#pragma once

#include <string>

#include "../item.h"

namespace server::core
{
	using namespace std;

	class logout_request : public item
	{
	public:
		logout_request();
		logout_request(const string& username);
		logout_request(const vector<char>& bytes);
		~logout_request();

		void set_username(const string& username);

		const string& get_username() const;

		vector<char> to_bytes();

	private:
		string _username;
	};
}