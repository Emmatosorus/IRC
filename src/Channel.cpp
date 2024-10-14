#include "../include/Channel.hpp"
#include <algorithm>
#include <sys/socket.h>

Channel::Channel(int creator_fd, std::string name)
    : creator_fd(creator_fd),
      name(name),
      topic(),
	  topic_starter_nickname(),
	  topic_timestampt(),
      subscribed_users_fd(1, creator_fd),
      channel_operators_fd(1, creator_fd),
	  password(""),
	  user_limit(-1), 
	  is_password_mode(false),
	  is_user_limit_mode(false),
      is_invite_only_mode(false),
      is_const_topic_mode(false)
{
    return;
}

Channel::Channel(const Channel& c)
    : creator_fd(c.creator_fd),
      name(c.name),
      topic(c.topic),
	  topic_starter_nickname(c.topic_starter_nickname),
	  topic_timestampt(c.topic_timestampt),
      subscribed_users_fd(c.subscribed_users_fd),
      channel_operators_fd(c.channel_operators_fd),
	  password(c.password),
	  user_limit(c.user_limit), 
	  is_password_mode(c.is_password_mode),
	  is_user_limit_mode(c.is_user_limit_mode),
      is_invite_only_mode(c.is_invite_only_mode),
      is_const_topic_mode(c.is_invite_only_mode)
{
	return;
}

Channel& Channel::operator=(const Channel& c)
{
	if (this == &c)
		return *this;
	creator_fd = c.creator_fd;
	name = c.name;
	topic = c.topic;
	topic_starter_nickname = c.topic_starter_nickname;
	topic_timestampt = c.topic_timestampt,
	subscribed_users_fd = c.subscribed_users_fd;
	channel_operators_fd = c.channel_operators_fd;
	password = c.password;
	user_limit = c.user_limit; 
	is_password_mode = c.is_password_mode;
	is_user_limit_mode = c.is_user_limit_mode;
	is_invite_only_mode = c.is_invite_only_mode;
	is_const_topic_mode = c.is_invite_only_mode;
	return *this;
}

Channel::~Channel() { return; }

bool Channel::is_subscribed(int fd)
{
	return std::find(subscribed_users_fd.begin(), subscribed_users_fd.end(), fd) != subscribed_users_fd.end();
}

bool Channel::is_operator(int fd)
{
	return std::find(channel_operators_fd.begin(), channel_operators_fd.end(), fd) != channel_operators_fd.end();
}

void Channel::send_msg(const std::string& msg)
{
	std::string total = msg + "\r\n";
	for (std::vector<int>::const_iterator it = subscribed_users_fd.begin();
		it != subscribed_users_fd.end(); it++)
	{
		send(*it, total.c_str(), total.size(), MSG_CONFIRM);
	}
}

void Channel::send_msg_except(int fd, const std::string& msg)
{
	std::string total = msg + "\r\n";
	for (std::vector<int>::const_iterator it = subscribed_users_fd.begin();
		it != subscribed_users_fd.end(); it++)
	{
		if (*it != fd)
			send(*it, total.c_str(), total.size(), MSG_CONFIRM);
	}
}
