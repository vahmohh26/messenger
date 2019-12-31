#include "message_response.h"

#include <sstream>

#include <boost/iostreams/stream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace server::core
{
	message_response::message_response()
	{
		_type = type::message;
		_result = result::failure;
	}

	message_response::message_response(result result, const string& username, const string& message) : message_response()
	{
		set_result(result);
		set_username(username);
		set_message(message);
	}

	message_response::message_response(const vector<char>& bytes) : message_response()
	{
		using namespace boost;

		iostreams::stream<iostreams::array_source> stream(iostreams::array_source(bytes.data(), bytes.size()));
		archive::text_iarchive text_iarchive(stream);
		text_iarchive >> _type;
		text_iarchive >> _result;
		text_iarchive >> _username;
		text_iarchive >> _message;
	}

	message_response::~message_response()
	{
	}

	void message_response::set_result(result result)
	{
		_result = result;
	}

	void message_response::set_username(const string& username)
	{
		_username = username;
	}

	void message_response::set_message(const string& message)
	{
		_message = message;
	}

	message_response::result message_response::get_result()
	{
		return _result;
	}

	const string& message_response::get_username() const
	{
		return _username;
	}

	const string& message_response::get_message() const
	{
		return _message;
	}

	vector<char> message_response::to_bytes()
	{
		using namespace boost;

		stringstream stringstream;
		archive::text_oarchive text_oarchive(stringstream);
		text_oarchive << _type;
		text_oarchive << _result;
		text_oarchive << _username;
		text_oarchive << _message;

		auto string = stringstream.str();

		return { string.c_str(), string.c_str() + string.size() };
	}
}