#pragma once

#include <string>

#include "../item.h"

namespace server::core
{
	using namespace std;

	class login_request : public item
	{
	public:
		login_request();
		login_request(const string& username);
		login_request(const vector<char>& bytes);
		~login_request();

		void set_username(const string& username);

		const string& get_username() const;

		vector<char> to_bytes();

	private:
		string _username;
	};
}