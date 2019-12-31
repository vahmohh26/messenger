#pragma once

#include "protocol/protocol.h"
#include "session/session.h"

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
		thread server_thread;
		thread requester_thread;
		thread responder_thread;
		atomic_bool server_thread_stop;
		atomic_bool requester_thread_stop;
		atomic_bool responder_thread_stop;
		protocol protocol;
		session session;
		queue<package> requested_packages;
		queue<package> responded_packages;
	};
}