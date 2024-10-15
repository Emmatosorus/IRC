#include "../../include/Server.hpp"


/* MODE <target> [<modestring> [<mode arguments>...]] */
void Server::_mode(PollfdIterator it, const std::vector<std::string>& args)
{
    // verifiy arguments
    // Parse arguments
    // Make sure that client is operator
}

/* toggles invite only */
void  Server::_mode_i(bool  is_off, Channel & channel, Client & client)
{
    if (is_off)
    {
        channel.is_invite_only_mode = false;
        return;
    }
    channel.is_invite_only_mode = true;
}

/* toggles operator priviliges to change topic */
void  Server::_mode_t(bool  is_off, Channel & channel, Client & client)
{
    if (is_off)
    {
        channel.is_const_topic_mode = false;
        return;
    }
    channel.is_const_topic_mode = true;
}

/* Set/removes channel password */
void  Server::_mode_k(bool  is_remove, std::string & args, Channel & channel, Client & client)
{
    if (is_remove)
    {
        channel.is_password_mode = false;
        channel.password.clear();
        return;
    }
    channel.is_password_mode = true;
    channel.password = args;
}

/* Give/remove operator priviliges */
void  Server::_mode_o(bool  is_remove, std::string & args, Channel & channel, Client & client)
{
    ClientIterator target_it = _find_client_by_nickname(args);
    if (target_it == m_clients.end())
    {
        // TODO send bad argument
    }
    if (!channel.is_subscribed(target_it->second.fd))
    {
        // TODO send target not subscribed
    }
    Client& target = target_it->second;
    if (is_remove)
    {
        channel.channel_operators_fd.push_back(target.fd);
    }
}

/* set/removes user limit on channel */
void  Server::_mode_l(bool  is_remove, std::string & args, Channel & channel, Client & client)
{

}
