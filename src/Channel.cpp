#include "../include/Channel.hpp"
#include "../include/Client.hpp"
#include "../include/utils.hpp"
#include <algorithm>
#include <map>
#include <sys/socket.h>

Channel::Channel()
    : creator_fd(-1),
      created_timestamp(0),
      name(""),
      topic(),
      topic_starter_nickname(),
      topic_timestampt(),
      subscribed_users_fd(1, creator_fd),
      channel_operators_fd(1, creator_fd),
      invited_users_fd(),
      password(""),
      user_limit(-1),
      is_password_mode(false),
      is_user_limit_mode(false),
      is_invite_only_mode(false),
      is_const_topic_mode(false),
      is_no_external_messages_mode(true)
{
    return;
}

Channel::Channel(int creator_fd, std::string name)
    : creator_fd(creator_fd),
      name(name),
      topic(),
      topic_starter_nickname(),
      topic_timestampt(),
      subscribed_users_fd(1, creator_fd),
      channel_operators_fd(1, creator_fd),
      invited_users_fd(),
      password(""),
      user_limit(-1),
      is_password_mode(false),
      is_user_limit_mode(false),
      is_invite_only_mode(false),
      is_const_topic_mode(false),
      is_no_external_messages_mode(true)
{
    this->created_timestamp = long_to_str(time(NULL));
    return;
}

Channel::Channel(const Channel& c)
    : creator_fd(c.creator_fd),
      created_timestamp(c.created_timestamp),
      name(c.name),
      topic(c.topic),
      topic_starter_nickname(c.topic_starter_nickname),
      topic_timestampt(c.topic_timestampt),
      subscribed_users_fd(c.subscribed_users_fd),
      channel_operators_fd(c.channel_operators_fd),
      invited_users_fd(c.invited_users_fd),
      password(c.password),
      user_limit(c.user_limit),
      is_password_mode(c.is_password_mode),
      is_user_limit_mode(c.is_user_limit_mode),
      is_invite_only_mode(c.is_invite_only_mode),
      is_const_topic_mode(c.is_invite_only_mode),
      is_no_external_messages_mode(c.is_no_external_messages_mode)
{
    return;
}

Channel& Channel::operator=(const Channel& c)
{
    if (this == &c)
        return *this;
    creator_fd = c.creator_fd;
    created_timestamp = c.created_timestamp;
    name = c.name;
    topic = c.topic;
    topic_starter_nickname = c.topic_starter_nickname;
    topic_timestampt = c.topic_timestampt, subscribed_users_fd = c.subscribed_users_fd;
    channel_operators_fd = c.channel_operators_fd;
    invited_users_fd = c.invited_users_fd;
    password = c.password;
    user_limit = c.user_limit;
    is_password_mode = c.is_password_mode;
    is_user_limit_mode = c.is_user_limit_mode;
    is_invite_only_mode = c.is_invite_only_mode;
    is_const_topic_mode = c.is_invite_only_mode;
    is_no_external_messages_mode = c.is_no_external_messages_mode;
    return *this;
}

Channel::~Channel() { return; }

bool Channel::is_subscribed(int fd) const
{
    return std::find(subscribed_users_fd.begin(), subscribed_users_fd.end(), fd) !=
           subscribed_users_fd.end();
}

bool Channel::is_operator(int fd) const
{
    return std::find(channel_operators_fd.begin(), channel_operators_fd.end(), fd) !=
           channel_operators_fd.end();
}

bool Channel::is_invited(int fd) const
{
    return std::find(invited_users_fd.begin(), invited_users_fd.end(), fd) !=
           invited_users_fd.end();
}

void Channel::send_msg(const std::string& msg) const
{
    std::string total = msg + "\r\n";
    for (std::vector<int>::const_iterator it = subscribed_users_fd.begin();
         it != subscribed_users_fd.end(); it++)
    {
        send(*it, total.c_str(), total.size(), MSG_CONFIRM);
    }
}

void Channel::send_msg_to_operators(int fd, const std::string& msg) const
{
    std::string total = msg + "\r\n";
    for (std::vector<int>::const_iterator it = channel_operators_fd.begin();
         it != channel_operators_fd.end(); it++)
    {
        if (*it != fd)
            send(*it, total.c_str(), total.size(), MSG_CONFIRM);
    }
}

void Channel::send_msg_except(int fd, const std::string& msg) const
{
    std::string total = msg + "\r\n";
    for (std::vector<int>::const_iterator it = subscribed_users_fd.begin();
         it != subscribed_users_fd.end(); it++)
    {
        if (*it != fd)
            send(*it, total.c_str(), total.size(), MSG_CONFIRM);
    }
}

void Channel::remove_client(Client& client)
{
    std::vector<int>::iterator subscribed_user_it =
        std::find(subscribed_users_fd.begin(), subscribed_users_fd.end(), client.fd);
    std::vector<int>::iterator channel_operator_it =
        std::find(channel_operators_fd.begin(), channel_operators_fd.end(), client.fd);
    if (channel_operator_it != channel_operators_fd.end())
        channel_operators_fd.erase(channel_operator_it);
    subscribed_users_fd.erase(subscribed_user_it);
}

std::string Channel::get_modestring() const
{
    std::string modestring = "+";
    if (is_no_external_messages_mode)
        modestring += "n";
    if (is_invite_only_mode)
        modestring += "i";
    if (is_password_mode)
        modestring += "k";
    if (is_user_limit_mode)
        modestring += "l";
    if (is_const_topic_mode)
        modestring += "t";

    if (is_password_mode)
        modestring += " " + password;
    if (is_user_limit_mode)
        modestring += " " + long_to_str(user_limit);
    return modestring;
}

std::string Channel::get_list_of_clients(const std::map<int, Client> m_clients) const
{
    std::string list_of_clients;
    for (std::vector<int>::const_iterator it = subscribed_users_fd.begin();
         it != subscribed_users_fd.end(); it++)
    {
        const Client& client = m_clients.find(*it)->second;
        if (is_operator(client.fd))
            list_of_clients += "@";
        list_of_clients += client.nickname;
        if (it + 1 != subscribed_users_fd.end())
            list_of_clients += " ";
    }
    return list_of_clients;
}
