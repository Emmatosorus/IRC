#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
class Client
{
	public:
          Client();
          Client(int fd);
          Client(const Client &c);
          Client &operator=(const Client &c);
          ~Client();
          int fd;
          bool is_operator;
          std::string buf;
          std::string nickname;
          std::string username;
          std::string fullname;
};
#endif
