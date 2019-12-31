#pragma once

#include <cstdint>

namespace server::configuration
{
	namespace core
	{
		constexpr size_t servers_number = 3;
	}

	namespace protocol
	{
		constexpr uint16_t port = 6574;
		constexpr uint16_t buffer_size = 1024;
	}
}