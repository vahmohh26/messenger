#include "message_request.h"

#include <sstream>

#include <boost/iostreams/stream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace server::core
{
	message_request::message_request()
	{
		_type = type::message;
	}

	message_request::message_request(const string& username, const string& message) : message_request()
	{
		set_username(username);
		set_message(message);
	}

	message_request::message_request(const vector<char>& bytes) : message_request()
	{
		using namespace boost;

		iostreams::stream<iostreams::array_source> stream(iostreams::array_source(bytes.data(), bytes.size()));
		archive::text_iarchive text_iarchive(stream);
		text_iarchive >> _type;
		text_iarchive >> _username;
		text_iarchive >> _message;
	}

	message_request::~message_request()
	{
	}

	void message_request::set_username(const string& username)
	{
		_username = username;
	}

	void message_request::set_message(const string& message)
	{
		_message = message;
	}

	const string& message_request::get_username() const
	{
		return _username;
	}

	const string& message_request::get_message() const
	{
		return _message;
	}

	vector<char> message_request::to_bytes()
	{
		using namespace boost;

		stringstream stringstream;
		archive::text_oarchive text_oarchive(stringstream);
		text_oarchive << _type;
		text_oarchive << _username;
		text_oarchive << _message;

		auto string = stringstream.str();

		return { string.c_str(), string.c_str() + string.size() };
	}
}