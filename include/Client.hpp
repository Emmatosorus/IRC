#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
#include <ctime>
class Client
{
	public:
          Client();
          Client(int fd);
          Client(const Client &c);
          Client &operator=(const Client &c);
          ~Client();
          int fd;
		  time_t since_last_ping;
		  bool entered_password;
		  bool is_registered;
          bool is_operator;
          std::string buf;
		  std::string password;
          std::string nickname;
          std::string username;
          std::string fullname;
};
#endif
