#include "../../include/Server.hpp"
#include <algorithm>


/* https://modern.ircdocs.horse/#mode-message
 * Parameters: <target> [<modestring> [<mode arguments>...]] */
void Server::_mode(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients.find((*it)->fd)->second;
	if (args.size() < 2)
		return client.send_461("MODE");

    const std::string& channel_name = args[1];
    bool is_operator = false;	

	ChannelIterator target_channel = m_channels.find(channel_name);
	if (target_channel == m_channels.end())
		return client.send_403(args[1]);
	Channel& channel = target_channel->second;

    std::map<int, Client>::iterator client_it = m_clients.find((*it)->fd);
    if (_check_presence(target_channel, client_it, is_operator) != 0)
    {
        client.send_442(target_channel->second);
        return;
    }
    if (!is_operator)
    {
        client.send_482(target_channel->second);
        return;
    }

	if (args.size() == 2)
	{
        client.send_324(channel);
        client.send_329(channel);
        return;
	}
    
	// :dan!~h@localhost MODE #foobar -bl+i *@192.168.0.1
	size_t j = 3;
	bool is_add_mode = false;
	for (size_t i = 0; i < args[2].size(); i++)
	{
		switch(args[2][i])
		{
			case '-':
				is_add_mode = false;
				break;
			case '+':
				is_add_mode = true;
				break;
			case 'i':
				_mode_i(is_add_mode, channel);
				break;
			case 't':
				_mode_t(is_add_mode, channel);
				break;
			case 'k':
                if (j > args.size())
                {
                    client.send_696(channel.name, 'k', "", "No argument given");
                    return ;
                }
				_mode_k(is_add_mode, args[j], channel);
                if (is_add_mode)
                    j++;
                break;
			case 'o':
                if (j > args.size())
                {
                    client.send_696(channel.name, 'o', "", "No argument given");
                    return ;
                }
				_mode_o(is_add_mode, args[j], channel, client);
                if (is_add_mode)
                    j++;
				break;
			case 'l':
                if (j > args.size())
                {
                    client.send_696(channel.name, 'l', "", "No argument given");
                    return ;
                }
				_mode_l(is_add_mode, args[j], channel, client);
				if (is_add_mode)
                    j++;
                break;
			default:
				client.send_472(args[2][i]);
				return;
		}
	}
}

/* toggles invite only */
void  Server::_mode_i(bool  is_add_mode, Channel & channel)
{
    if (!is_add_mode)
    {
        channel.is_invite_only_mode = false;
        return;
    }
    channel.is_invite_only_mode = true;
}

/* toggles operator priviliges to change topic */
void Server::_mode_t(bool  is_add_mode, Channel & channel)
{
    if (!is_add_mode)
    {
        channel.is_const_topic_mode = false;
        return;
    }
    channel.is_const_topic_mode = true;
}

/* Set/removes channel password */
void  Server::_mode_k(bool  is_add_mode, const std::string & password, Channel & channel)
{
    if (!is_add_mode)
    {
        channel.is_password_mode = false;
        channel.password.clear();
        return;
    }
    channel.is_password_mode = true;
    channel.password = password;
}

/* Give/remove operator priviliges */
void  Server::_mode_o(bool  is_add_mode, const std::string & nickname, Channel & channel, Client & client)
{
    ClientIterator target_it = _find_client_by_nickname(nickname);
    if (target_it == m_clients.end())
    {
        client.send_696(channel.name, 'o', nickname, "Unknown user");
    }

	Client& target = target_it->second;
    if (!channel.is_subscribed(target.fd))
    {
        client.send_696(channel.name, 'o', nickname, "User is not subscribed to channel");
    }
    if (is_add_mode)
    {
        channel.channel_operators_fd.push_back(target.fd);
        return;
    }
    std::vector<int>::iterator it = std::find(channel.channel_operators_fd.begin(), channel.channel_operators_fd.end(), target.fd);
    channel.channel_operators_fd.erase(it);
}

/* set/removes user limit on channel */
void  Server::_mode_l(bool  is_add_mode, const std::string & user_limit, Channel & channel, Client & client)
{
    if (is_add_mode)
    {
        channel.is_user_limit_mode = true;
        size_t limit = std::atoi(user_limit.c_str());
        if (limit > 0)
        {
            channel.user_limit = limit;
            return;
        }
        client.send_696(channel.name, 'l', user_limit, "User limit must be greater than 0");
        return;
    }
    channel.is_user_limit_mode = false;
    channel.user_limit = 0;
}
