#include "login_request.h"

#include <sstream>

#include <boost/iostreams/stream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace server::core
{
	login_request::login_request()
	{
		_type = type::login;
	}

	login_request::login_request(const string& username) : login_request()
	{
		set_username(username);
	}

	login_request::login_request(const vector<char>& bytes) : login_request()
	{
		using namespace boost;

		iostreams::stream<iostreams::array_source> stream(iostreams::array_source(bytes.data(), bytes.size()));
		archive::text_iarchive text_iarchive(stream);
		text_iarchive >> _type;
		text_iarchive >> _username;
	}

	login_request::~login_request()
	{
	}

	void login_request::set_username(const string& username)
	{
		_username = username;
	}

	const string& login_request::get_username() const
	{
		return _username;
	}

	vector<char> login_request::to_bytes()
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