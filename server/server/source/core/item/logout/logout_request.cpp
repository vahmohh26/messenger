#include "logout_request.h"

#include <sstream>

#include <boost/iostreams/stream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace server::core
{
	logout_request::logout_request()
	{
		_type = type::logout;
	}

	logout_request::logout_request(const string& username) : logout_request()
	{
		set_username(username);
	}

	logout_request::logout_request(const vector<char>& bytes) : logout_request()
	{
		using namespace boost;

		iostreams::stream<iostreams::array_source> stream(iostreams::array_source(bytes.data(), bytes.size()));
		archive::text_iarchive text_iarchive(stream);
		text_iarchive >> _type;
		text_iarchive >> _username;
	}

	logout_request::~logout_request()
	{
	}

	void logout_request::set_username(const string& username)
	{
		_username = username;
	}

	const string& logout_request::get_username() const
	{
		return _username;
	}

	vector<char> logout_request::to_bytes()
	{
		using namespace boost;

		stringstream stringstream;
		archive::text_oarchive text_oarchive(stringstream);
		text_oarchive << _type;
		text_oarchive << _username;

		auto string = stringstream.str();

		return { string.c_str(), string.c_str() + string.size() };
	}
}