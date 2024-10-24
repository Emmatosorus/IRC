#include "../include/Client.hpp"
#include "../include/utils.hpp"
#include <algorithm>
#include <sys/socket.h>

Client::Client()
    : fd(-1),
      entered_password(false),
      is_registered(false),
      buf(""),
      password(""),
      nickname(""),
      username(""),
      fullname(""),
	  away_msg(""),
      channels()
{
    return;
}

Client::Client(int fd)
    : fd(fd),
      entered_password(false),
      is_registered(false),
      buf(""),
      password(""),
      nickname(""),
      username(""),
      fullname(""),
	  away_msg(""),
      channels()
{
    return;
}

Client::Client(const Client& c)
    : fd(c.fd),
      entered_password(c.entered_password),
      is_registered(c.is_registered),
      buf(c.buf),
      password(""),
      nickname(c.nickname),
      username(c.username),
      fullname(c.fullname),
	  away_msg(c.away_msg),
      channels(c.channels)
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
    buf = c.buf;
    password = c.password;
    nickname = c.nickname;
    username = c.username;
    fullname = c.fullname;
	away_msg = c.away_msg;
    channels = c.channels;
    return *this;
}

Client::~Client() { return; }

void Client::send_msg(const std::string& msg) const
{
    std::string total = msg + "\r\n";
	sendall(fd, total);
}

void Client::quit_channel(const std::string channel_name)
{
    std::vector<std::string>::iterator it =
        std::find(channels.begin(), channels.end(), channel_name);
    channels.erase(it);
}

std::string Client::_resolve_nickname()
{
    if (nickname == "" && username == "")
        return "guest";
    else if (nickname == "")
        return username;
    return nickname;
}
