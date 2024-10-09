#include "../include/Client.hpp"
#include <unistd.h>

Client::Client() : fd(-1), is_operator(false), buf(""), nickname(""), username(""), fullname("")
{
	return;
}

Client::Client(int fd) : fd(fd), is_operator(false), buf(""), nickname(""), username(""), fullname("")
{
	return;
}

Client::Client(const Client& c) : fd(c.fd), is_operator(c.is_operator), buf(c.buf), nickname(c.nickname), username(c.username), fullname(c.fullname)
{
	return;
}

Client& Client::operator=(const Client& c)
{
	fd = c.fd;
	is_operator = c.is_operator;
	buf = c.buf;
	nickname = c.nickname;
	username = c.username;
	fullname = c.fullname;
	return *this;
}

Client::~Client()
{
	return;
}
