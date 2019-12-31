#include "logout_response.h"

#include <sstream>

#include <boost/iostreams/stream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace server::core
{
	logout_response::logout_response()
	{
		_type = type::logout;
		_result = result::failure;
	}

	logout_response::logout_response(const result& result) : logout_response()
	{
		set_result(result);
	}

	logout_response::logout_response(const vector<char>& bytes) : logout_response()
	{
		using namespace boost;

		iostreams::stream<iostreams::array_source> stream(iostreams::array_source(bytes.data(), bytes.size()));
		archive::text_iarchive text_iarchive(stream);
		text_iarchive >> _type;
		text_iarchive >> _result;
	}

	logout_response::~logout_response()
	{
	}

	void logout_response::set_result(result result)
	{
		_result = result;
	}

	logout_response::result logout_response::get_result()
	{
		return _result;
	}

	vector<char> logout_response::to_bytes()
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