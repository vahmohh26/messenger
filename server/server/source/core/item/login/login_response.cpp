#include "login_response.h"

#include <sstream>

#include <boost/iostreams/stream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace server::core
{
	login_response::login_response()
	{
		_type = type::login;
	}

	login_response::login_response(const result& result) : login_response()
	{
		set_result(result);
	}

	login_response::login_response(const vector<char>& bytes) : login_response()
	{
		using namespace boost;

		iostreams::stream<iostreams::array_source> stream(iostreams::array_source(bytes.data(), bytes.size()));
		archive::text_iarchive text_iarchive(stream);
		text_iarchive >> _type;
		text_iarchive >> _result;
	}

	login_response::~login_response()
	{
	}

	void login_response::set_result(const result& result)
	{
		_result = result;
	}

	const login_response::result& login_response::get_result() const
	{
		return _result;
	}

	vector<char> login_response::to_bytes()
	{
		using namespace boost;

		stringstream stringstream;
		archive::text_oarchive text_oarchive(stringstream);
		text_oarchive << _type;
		text_oarchive << _result;

		auto string = stringstream.str();

		return { string.c_str(), string.c_str() + string.size() };
	}
}