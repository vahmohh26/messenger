#pragma once

#include "../item.h"

namespace server::core
{
	using namespace std;

	class login_response : public item
	{
	public:
		enum class result
		{
			success,
			failure
		};

		login_response();
		login_response(const result& result);
		login_response(const vector<char>& bytes);
		~login_response();

		void set_result(const result& result);

		const result& get_result() const;

		vector<char> to_bytes();

	protected:
		result _result;
	};
}