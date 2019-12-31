#pragma once

#include <string>

#include "../item.h"

namespace server::core
{
	using namespace std;

	class message_response : public item
	{
	public:
		enum class result
		{
			success,
			failure
		};

		message_response();
		message_response(result result, const string& username, const string& message);
		message_response(const vector<char>& bytes);
		~message_response();

		void set_result(result result);
		void set_username(const string& username);
		void set_message(const string& message);

		result get_result();
		const string& get_username() const;
		const string& get_message() const;

		vector<char> to_bytes();

	private:
		result _result;
		string _username;
		string _message;
	};
}