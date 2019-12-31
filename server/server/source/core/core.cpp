#include "core.h"

#include <mutex>
#include <optional>

#include <log/log.h>

#include "service/service.h"
#include "item/item.h"

namespace server::core
{
	mutex requested_packages_mutex;
	mutex responded_packages_mutex;

	core::core()
	{
		initialized = false;
		terminated = false;
		server_thread_stop = false;
		requester_thread_stop = false;
		responder_thread_stop = false;
	}

	core::~core()
	{
	}

	bool core::initialize()
	{
		if (initialized)
		{
			LOG();

			return false;
		}

		if (!protocol.initialize())
		{
			LOG();

			return false;
		}

		for (auto& server_thread : server_threads)
		{
			server_thread = thread(&core::server_thread_function, this);
		}

		requester_thread = thread(&core::requester_thread_function, this);
		responder_thread = thread(&core::responder_thread_function, this);

		initialized = true;

		return true;
	}

	bool core::terminate()
	{
		if (!initialized || terminated)
		{
			LOG();

			return false;
		}

		server_thread_stop = true;
		requester_thread_stop = true;
		responder_thread_stop = true;

		for (auto& server_thread : server_threads)
		{
			if (server_thread.joinable())
			{
				server_thread.join();
			}
		}

		if (requester_thread.joinable())
		{
			requester_thread.join();
		}

		if (responder_thread.joinable())
		{
			responder_thread.join();
		}

		if (!protocol.terminate())
		{
			LOG();
		}

		terminated = true;

		return true;
	}

	void core::server_thread_function()
	{
		optional<package> requested_package;
		queue<package> responded_package;

		while (!server_thread_stop)
		{
			requested_packages_mutex.lock();

			if (!requested_packages.empty())
			{
				requested_package = requested_packages.front();
				requested_packages.pop();
			}

			requested_packages_mutex.unlock();

			if (requested_package.has_value())
			{
				switch (item(requested_package.value().get_buffer()).get_type())
				{
				case item::type::login:
					responded_package = login(session, requested_package.value());
				case item::type::logout:
					responded_package = logout(session, requested_package.value());
					break;
				default:
					break;
				}

				while (!responded_package.empty())
				{
					responded_packages_mutex.lock();
					responded_packages.push(responded_package.front());
					responded_packages_mutex.unlock();

					responded_package.pop();
				}

				requested_package.reset();
			}
		}
	}

	void core::requester_thread_function()
	{
		queue<package> packages;

		while (!requester_thread_stop)
		{
			if (protocol.receive(packages))
			{
				while (!packages.empty())
				{
					requested_packages_mutex.lock();
					requested_packages.push(packages.front());
					requested_packages_mutex.unlock();

					packages.pop();
				}
			}
		}
	}

	void core::responder_thread_function()
	{
		optional<package> package;

		while (!responder_thread_stop)
		{
			responded_packages_mutex.lock();

			if (!responded_packages.empty())
			{
				package = responded_packages.front();
				responded_packages.pop();
			}

			responded_packages_mutex.unlock();

			if (package.has_value())
			{
				protocol.send(package.value());

				package.reset();
			}
		}
	}
}