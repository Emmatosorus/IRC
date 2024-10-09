#include "../include/Channel.hpp"

Channel::Channel(int creator_fd, std::string name) : creator_fd(creator_fd), name(name), topic(), subscribed_users_fd(), channel_operators_fd(), password_mode(std::make_pair(false, "")), user_limit_mode(std::make_pair(false, 0)), is_invite_only_mode(false), is_const_topic_mode(false)
{
	return;
}

Channel::~Channel()
{
	return;
}
