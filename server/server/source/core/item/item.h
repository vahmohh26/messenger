#pragma once

#include <vector>

namespace server::core
{
	using namespace std;

	class item
	{
	public:
		enum class type
		{
			unknown,
			login,
			logout
		};

		item();
		item(type type);
		item(const vector<char>& bytes);
		~item();

		void set_type(type type);

		type get_type();

		vector<char> to_bytes();

	protected:
		type _type;
	};
}