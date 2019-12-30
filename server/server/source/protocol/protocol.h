#pragma once

#include <thread>
#include <atomic>
#include <unordered_map>
#include <queue>

#include <WinSock2.h>

#include "package/package.h"

namespace server::protocol
{
	using namespace std;

	class protocol
	{
	public:
		protocol();
		~protocol();

		bool initialize();
		bool terminate();

		bool receive(queue<package>& packages);
		bool send(const package& package);

	private:
		void receiver_thread_function();
		void sender_thread_function();

		SOCKET ip_to_socket(const string& ip);

		static string addr_to_ip(const SOCKADDR_IN& addr);

		bool initialized;
		bool terminated;
		thread receiver_thread;
		thread sender_thread;
		atomic_bool receiver_thread_stop;
		atomic_bool sender_thread_stop;
		SOCKET server_socket;
		unordered_map<string, SOCKET> client_sockets;
		queue<package> received_packages;
		queue<package> sent_packages;
	};
}