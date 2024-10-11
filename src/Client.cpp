#include "../include/Client.hpp"
#include <unistd.h>

Client::Client() : fd(-1), entered_password(false), is_registered(false), is_operator(false), buf(""), nickname(""), username(""), fullname("")
{
	return;
}

Client::Client(int fd) : fd(fd), entered_password(false), is_registered(false), is_operator(false), buf(""), nickname(""), username(""), fullname("")
{
	return;
}

Client::Client(const Client& c) : fd(c.fd), entered_password(c.entered_password), is_registered(c.is_registered), is_operator(c.is_operator), buf(c.buf), nickname(c.nickname), username(c.username), fullname(c.fullname)
{
	return;
}

Client& Client::operator=(const Client& c)
{
	fd = c.fd;
	is_operator = c.is_operator;
	buf = c.buf;
	entered_password = c.entered_password;
	is_registered = c.is_registered;
	password = c.password;
	nickname = c.nickname;
	username = c.username;
	fullname = c.fullname;
	return *this;
}

Client::~Client()
{
	return;
}
