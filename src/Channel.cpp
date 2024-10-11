#include "../include/Channel.hpp"

Channel::Channel(int creator_fd, std::string name)
    : creator_fd(creator_fd),
      name(name),
      topic(),
      subscribed_users_fd(),
      channel_operators_fd(),
      password_mode(std::make_pair(false, "")),
      user_limit_mode(std::make_pair(false, 0)),
      is_invite_only_mode(false),
      is_const_topic_mode(false)
{
    return;
}

Channel::Channel(const Channel& c)
    : creator_fd(c.creator_fd),
      name(c.name),
      topic(c.topic),
      subscribed_users_fd(c.subscribed_users_fd),
      channel_operators_fd(c.channel_operators_fd),
      password_mode(c.password_mode),
      user_limit_mode(c.user_limit_mode),
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
	subscribed_users_fd = c.subscribed_users_fd;
	channel_operators_fd = c.channel_operators_fd;
	password_mode = c.password_mode;
	user_limit_mode = c.user_limit_mode;
	is_invite_only_mode = c.is_invite_only_mode;
	is_const_topic_mode = c.is_invite_only_mode;
	return *this;
}

Channel::~Channel() { return; }
