#include "session.h"

#include <mutex>

namespace server::core
{
	mutex ips_usernames_mutex;

	session::session()
	{
	}

	session::~session()
	{
	}

	void session::add(const string& ip, const string& username)
	{
		ips_usernames_mutex.lock();
		ips.insert_or_assign(username, ip);
		usernames.insert_or_assign(ip, username);
		ips_usernames_mutex.unlock();
	}

	void session::remove_by_ip(const string& ip)
	{
		auto username = ip_to_username(ip);

		ips_usernames_mutex.lock();
		ips.erase(username);
		usernames.erase(ip);
		ips_usernames_mutex.unlock();
	}

	void session::remove_by_username(const string& username)
	{
		auto ip = username_to_ip(username);

		ips_usernames_mutex.lock();
		ips.erase(username);
		usernames.erase(ip);
		ips_usernames_mutex.unlock();
	}

	string session::ip_to_username(const string& ip)
	{
		lock_guard<mutex> lock(ips_usernames_mutex);

		auto username_iterator = usernames.find(ip);
		if (username_iterator == usernames.end())
		{
			return {};
		}

		return username_iterator->second;
	}

	string session::username_to_ip(const string& username)
	{
		lock_guard<mutex> lock(ips_usernames_mutex);

		auto ip_iterator = ips.find(username);
		if (ip_iterator == ips.end())
		{
			return {};
		}

		return ip_iterator->second;
	}
}