#include "service.h"

#include "../item/login/login_request.h"
#include "../item/login/login_response.h"
#include "../item/logout/logout_request.h"
#include "../item/logout/logout_response.h"

namespace server::core
{
	queue<package> login(const package& requested_package)
	{
		queue<package> responded_packages;

		if (login_request(requested_package.get_buffer()).get_username() == "vahid")
		{
			responded_packages.push({ requested_package.get_ip(), login_response(login_response::result::success).to_bytes() });
		}
		else
		{
			responded_packages.push({ requested_package.get_ip(), login_response(login_response::result::failure).to_bytes() });
		}

		return move(responded_packages);
	}

	queue<package> logout(const package& requested_package)
	{
		queue<package> responded_packages;

		if (login_request(requested_package.get_buffer()).get_username() == "vahid")
		{
			responded_packages.push({ requested_package.get_ip(), login_response(login_response::result::success).to_bytes() });
		}
		else
		{
			responded_packages.push({ requested_package.get_ip(), login_response(login_response::result::failure).to_bytes() });
		}

		return move(responded_packages);
	}
}