#include "item.h"

#include <sstream>

#include <boost/iostreams/stream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace server::core
{
	item::item()
	{
		_type = type::unknown;
	}

	item::item(type type) : item()
	{
		set_type(type);
	}

	item::item(const vector<char>& bytes) : item()
	{
		using namespace boost;

		iostreams::stream<iostreams::array_source> stream(iostreams::array_source(bytes.data(), bytes.size()));
		archive::text_iarchive text_iarchive(stream);
		text_iarchive >> _type;
	}

	item::~item()
	{
	}

	void item::set_type(type type)
	{
		_type = type;
	}

	item::type item::get_type()
	{
		return _type;
	}

	vector<char> item::to_bytes()
	{
		using namespace boost;

		stringstream stringstream;
		archive::text_oarchive text_oarchive(stringstream);
		text_oarchive << _type;

		auto string = stringstream.str();

		return { string.c_str(), string.c_str() + string.size() };
	}
}