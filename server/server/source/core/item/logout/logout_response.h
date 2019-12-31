#pragma once

#include "../item.h"

namespace server::core
{
	using namespace std;

	class logout_response : public item
	{
	public:
		enum class result
		{
			success,
			failure
		};

		logout_response();
		logout_response(const result& result);
		logout_response(const vector<char>& bytes);
		~logout_response();

		void set_result(result result);

		result get_result();

		vector<char> to_bytes();

	private:
		result _result;
	};
}