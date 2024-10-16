#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Channel.hpp"
#include <ctime>
#include <string>
class Client
{
	public:
		Client();
		Client(int fd);
		Client(const Client& c);
		Client& operator=(const Client& c);
		~Client();
		int fd;
		bool entered_password;
		bool is_registered;
		std::string buf;
		std::string password;
		std::string nickname;
		std::string username;
		std::string fullname;

		void send_msg(const std::string& msg);
		void send_001();
	    void send_324(const Channel& channel);
	    void send_329(const Channel& channel);
        void send_331(const Channel& channel);
		void send_332(const Channel& channel);
		void send_333(const Channel& channel);
		void send_341(const Channel& channel, const std::string & client_nick);
		void send_353(const Channel& channel, const std::string& list_of_clients);
		void send_366(const std::string& channel_name);
		void send_401(const std::string& nonexistent_nickname);
		void send_403(const std::string& channel_name);
		void send_407(const std::string& abort_msg);
		void send_411();
		void send_412();
		void send_417();
		void send_421(const std::string& command);
		void send_441(const std::string& channel_name);
		void send_442(const Channel& channel);
		void send_443(const Channel& channel);
		void send_448(const std::string& channel_name, const std::string& reason);
		void send_451();
		void send_461(const std::string& command);
		void send_462();
		void send_471(const Channel& channel);
        void send_472(char modechar);
		void send_473(const Channel& channel);
		void send_475(const Channel& channel);
		void send_482(const Channel& channel);
        void send_696(std::string target_name, char modechar, std::string parameter, std::string description);
		void send_1003();
};
#endif
