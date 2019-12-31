#pragma once

#include <string>

#include "../item.h"

namespace server::core
{
	using namespace std;

	class message_request : public item
	{
	public:
		message_request();
		message_request(const string& username, const string& message);
		message_request(const vector<char>& bytes);
		~message_request();

		void set_username(const string& username);
		void set_message(const string& message);

		const string& get_username() const;
		const string& get_message() const;

		vector<char> to_bytes();

	private:
		string _username;
		string _message;
	};
}