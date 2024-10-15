#include "../../include/Server.hpp"


/* https://modern.ircdocs.horse/#mode-message
 * Parameters: <target> [<modestring> [<mode arguments>...]] */
void Server::_mode(PollfdIterator it, const std::vector<std::string>& args)
{
	Client& client = m_clients.find(it->fd)->second;
	if (args.size() < 2)
		return client.send_461("MODE");

	// TODO: validate channel name

	ChannelIterator channel_it = m_channels.find(args[1]);
	if (channel_it == m_channels.end())
		return client.send_403(args[1]);

	Channel& channel = channel_it->second;
	if (args.size() == 2)
	{
		// RPL_CHANNELMODEIS (324)  
		// RPL_CREATIONTIME (329)
	}
	// :dan!~h@localhost MODE #foobar -bl+i *@192.168.0.1
	int j = 3;
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
				_mode_i(is_add_mode, channel, client);
				break;
			case 't':
				_mode_t(is_add_mode, channel, client);
				break;
			case 'k':
				_mode_k(is_add_mode, channel, client);
				break;
			case 'o':
				_mode_o(is_add_mode, channel, client);
				break;
			case 'l':
				_mode_l(is_add_mode, channel, client);
				break;
			default:
				// TODO: handle default case
				// ERR_UNKNOWNMODE (472)
				return;
		}
	}
	// handle modstring and the arguments
	// ERR_INVALIDMODEPARAM (696)
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
void  Server::_mode_k(bool  is_off, std::string & password, Channel & channel, Client & client)
{
    if (is_off)
    {
        channel.is_password_mode = false;
        channel.password.clear();
        return;
    }
    channel.is_password_mode = true;
    channel.password = password;
}

/* Give/remove operator priviliges */
void  Server::_mode_o(bool  is_off, std::string & nickname, Channel & channel, Client & client)
{
    ClientIterator target_it = _find_client_by_nickname(nickname);
    if (target_it == m_clients.end())
    {
        // TODO send bad argument
    }

	Client& target = target_it->second;
    if (!channel.is_subscribed(target.fd))
    {
        // TODO send target not subscribed
    }
    if (is_off)
    {
        channel.channel_operators_fd.push_back(target.fd);
    }
}

/* set/removes user limit on channel */
void  Server::_mode_l(bool  is_off, std::string & args, Channel & channel, Client & client)
{
	
}
