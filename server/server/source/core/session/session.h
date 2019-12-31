#pragma once

#include <unordered_map>

namespace server::core
{
	using namespace std;

	class session
	{
	public:
		session();
		~session();

		void add(const string& ip, const string& username);
		void remove_by_ip(const string& ip);
		void remove_by_username(const string& username);

		string ip_to_username(const string& ip);
		string username_to_ip(const string& username);

	private:
		unordered_map<string, string> ips;
		unordered_map<string, string> usernames;
	};
}