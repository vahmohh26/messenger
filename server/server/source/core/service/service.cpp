#include "service.h"

#include "../item/login/login_request.h"
#include "../item/login/login_response.h"
#include "../item/logout/logout_request.h"
#include "../item/logout/logout_response.h"
#include "../item/message/message_request.h"
#include "../item/message/message_response.h"

namespace server::core
{
	queue<package> login(session& session, const package& requested_package)
	{
		queue<package> responded_packages;
		auto request = login_request(requested_package.get_buffer());

		if (session.ip_to_username(requested_package.get_ip()).empty())
		{
			session.add(requested_package.get_ip(), request.get_username());

			responded_packages.push({ requested_package.get_ip(), login_response(login_response::result::success).to_bytes() });
		}
		else
		{
			responded_packages.push({ requested_package.get_ip(), login_response(login_response::result::failure).to_bytes() });
		}

		return move(responded_packages);
	}

	queue<package> logout(session& session, const package& requested_package)
	{
		queue<package> responded_packages;
		auto request = logout_request(requested_package.get_buffer());

		if (session.ip_to_username(requested_package.get_ip()).empty())
		{
			responded_packages.push({ requested_package.get_ip(), logout_response(logout_response::result::failure).to_bytes() });
		}
		else
		{
			session.remove_by_ip(requested_package.get_ip());

			responded_packages.push({ requested_package.get_ip(), logout_response(logout_response::result::success).to_bytes() });
		}

		return move(responded_packages);
	}

	queue<package> message(session& session, const package& requested_package)
	{
		queue<package> responded_packages;
		auto request = message_request(requested_package.get_buffer());

		auto source_username = session.ip_to_username(requested_package.get_ip());
		auto destination_ip = session.username_to_ip(request.get_username());

		if (source_username.empty() || destination_ip.empty())
		{
			responded_packages.push({ requested_package.get_ip(), message_response(message_response::result::failure, request.get_username(), request.get_message()).to_bytes() });
		}
		else
		{
			responded_packages.push({ destination_ip, message_response(message_response::result::success, source_username, request.get_message()).to_bytes() });
			responded_packages.push({ requested_package.get_ip(), message_response(message_response::result::success, request.get_username(), request.get_message()).to_bytes() });
		}

		return move(responded_packages);
	}
}