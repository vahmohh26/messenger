#include "protocol.h"

#include <mutex>
#include <optional>

#include <WS2tcpip.h>

#include <configuration/configuration.h>
#include <log/log.h>

namespace server::protocol
{
	mutex client_sockets_mutex;
	mutex received_packages_mutex;
	mutex sent_packages_mutex;

	protocol::protocol()
	{
		initialized = false;
		terminated = false;
		receiver_thread_stop = false;
		sender_thread_stop = false;
		server_socket = INVALID_SOCKET;
	}

	protocol::~protocol()
	{
	}

	bool protocol::initialize()
	{
		if (initialized)
		{
			return false;
		}

		WSADATA wsadata;
		if (::WSAStartup(MAKEWORD(2, 2), &wsadata) != NO_ERROR)
		{
			LOG(to_string(::WSAGetLastError()));

			return false;
		}

		server_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (server_socket == INVALID_SOCKET)
		{
			LOG(to_string(::WSAGetLastError()));

			return false;
		}

		u_long mode = 1;
		if (::ioctlsocket(server_socket, FIONBIO, &mode) == SOCKET_ERROR)
		{
			LOG(to_string(::WSAGetLastError()));

			return false;
		}

		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(configuration::protocol::port);

		if (::bind(server_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			LOG(to_string(::WSAGetLastError()));

			return false;
		}

		if (::listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
		{
			LOG(to_string(::WSAGetLastError()));

			return false;
		}

		receiver_thread = thread(&protocol::receiver_thread_function, this);
		sender_thread = thread(&protocol::receiver_thread_function, this);

		initialized = true;

		return true;
	}

	bool protocol::terminate()
	{
		if (!initialized || terminated)
		{
			return false;
		}

		receiver_thread_stop = true;
		sender_thread_stop = true;

		if (receiver_thread.joinable())
		{
			receiver_thread.join();
		}

		if (sender_thread.joinable())
		{
			sender_thread.join();
		}

		for (auto const& [ip, socket] : client_sockets)
		{
			if (::closesocket(socket) == SOCKET_ERROR)
			{
				LOG(to_string(::WSAGetLastError()));
			}
		}

		if (::closesocket(server_socket) == SOCKET_ERROR)
		{
			LOG(to_string(::WSAGetLastError()));
		}

		if (::WSACleanup() != NO_ERROR)
		{
			LOG(to_string(::WSAGetLastError()));
		}

		terminated = true;

		return true;
	}

	bool protocol::receive(queue<package>& packages)
	{
		if (!initialized || terminated)
		{
			return false;
		}

		received_packages_mutex.lock();
		packages = move(received_packages);
		received_packages_mutex.unlock();

		return true;
	}

	bool protocol::send(const package& package)
	{
		if (!initialized || terminated)
		{
			return false;
		}

		sent_packages_mutex.lock();
		sent_packages.push(package);
		sent_packages_mutex.unlock();

		return true;
	}

	void protocol::receiver_thread_function()
	{
		SOCKET socket;
		SOCKADDR_IN addr;
		int addr_size = sizeof(addr);

		int receive;
		char buffer[configuration::protocol::buffer_size];

		while (!receiver_thread_stop)
		{
			socket = ::accept(server_socket, (SOCKADDR*)&addr, &addr_size);
			if (socket != INVALID_SOCKET)
			{
				client_sockets_mutex.lock();
				client_sockets.insert_or_assign(addr_to_ip(addr), socket);
				client_sockets_mutex.unlock();
			}

			client_sockets_mutex.lock();

			for (auto const& [ip, socket] : client_sockets)
			{
				receive = ::recv(socket, buffer, static_cast<int>(configuration::protocol::buffer_size), 0);
				if (receive > 0)
				{
					received_packages_mutex.lock();
					received_packages.push({ ip, vector<char>(buffer, buffer + receive) });
					received_packages_mutex.unlock();
				}
			}

			client_sockets_mutex.unlock();
		}
	}

	void protocol::sender_thread_function()
	{
		optional<package> package;
		SOCKET socket;

		while (!sender_thread_stop)
		{
			sent_packages_mutex.lock();

			if (!sent_packages.empty())
			{
				package = sent_packages.front();
				sent_packages.pop();
			}

			sent_packages_mutex.unlock();

			if (package.has_value())
			{
				socket = ip_to_socket(package.value().get_ip());
				if (socket != INVALID_SOCKET)
				{
					if (::send(socket, package.value().get_buffer().data(), static_cast<int>(package.value().get_buffer().size()), 0) == SOCKET_ERROR)
					{
						LOG(to_string(::WSAGetLastError()));
					}
				}

				package.reset();
			}
		}
	}

	SOCKET protocol::ip_to_socket(const string& ip)
	{
		lock_guard<mutex> lock(client_sockets_mutex);

		auto socket_iterator = client_sockets.find(ip);
		if (socket_iterator == client_sockets.end())
		{
			return INVALID_SOCKET;
		}

		return socket_iterator->second;
	}

	string protocol::addr_to_ip(const SOCKADDR_IN& addr)
	{
		char ip[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);

		return ip;
	}
}