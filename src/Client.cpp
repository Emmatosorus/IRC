#include "../include/Client.hpp"
#include <sys/socket.h>

Client::Client()
    : fd(-1),
      entered_password(false),
      is_registered(false),
      is_operator(false),
      buf(""),
      password(""),
      nickname(""),
      username(""),
      fullname("")
{
    return;
}

Client::Client(int fd)
    : fd(fd),
      entered_password(false),
      is_registered(false),
      is_operator(false),
      buf(""),
      password(""),
      nickname(""),
      username(""),
      fullname("")
{
    return;
}

Client::Client(const Client& c)
    : fd(c.fd),
      entered_password(c.entered_password),
      is_registered(c.is_registered),
      is_operator(c.is_operator),
      buf(c.buf),
      password(""),
      nickname(c.nickname),
      username(c.username),
      fullname(c.fullname)
{
    return;
}

Client& Client::operator=(const Client& c)
{
	if (this == &c)
		return *this;
    fd = c.fd;
    entered_password = c.entered_password;
    is_registered = c.is_registered;
    is_operator = c.is_operator;
    buf = c.buf;
    password = c.password;
    nickname = c.nickname;
    username = c.username;
    fullname = c.fullname;
    return *this;
}

Client::~Client() { return; }

void Client::send_msg(const std::string& msg)
{
	std::string total = msg + "\r\n";
	send(fd, total.c_str(), total.size(), MSG_CONFIRM);
}
