#pragma once

#include <array>

#include <configuration/configuration.h>
#include <protocol/protocol.h>

namespace server::core
{
	using namespace std;

	class core
	{
	public:
		core();
		~core();

		bool initialize();
		bool terminate();

	private:
		void server_thread_function();
		void requester_thread_function();
		void responder_thread_function();

		bool initialized;
		bool terminated;
		array<thread, configuration::core::servers_number> server_threads;
		thread requester_thread;
		thread responder_thread;
		atomic_bool server_thread_stop;
		atomic_bool requester_thread_stop;
		atomic_bool responder_thread_stop;
		protocol::protocol protocol;
		queue<protocol::package> requested_packages;
		queue<protocol::package> responded_packages;
	};
}