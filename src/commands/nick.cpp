#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#nick-message
 * Parameters: <nickname> */
void Server::_nick(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients[(*it)->fd];
    if (args.size() < 2)
        return client.send_431();

    const std::string& nickname = args[1];
    if (std::isalpha(args[1][0]) == 0)
        return client.send_432(nickname, "First character of nickname must be a-z or A-Z");

    if (nickname.find_first_of("#:,*?!@.\t\r\n ") != std::string::npos)
        return client.send_432(nickname,
                               "Nickname contains invalid characters : #:,*?!@.\\t\\r\\n ");

    ClientIterator potential_nickname_holder = _find_client_by_nickname(nickname);
	if (potential_nickname_holder != m_clients.end() && potential_nickname_holder->second.fd != client.fd)
		return client.send_433(nickname);

	std::string old_nickname = client.nickname;
    client.nickname = nickname;
    if (client.username != "")
    {
        if (client.password != m_password)
            return client.send_464();

        if (!client.is_registered)
        {
            client.is_registered = true;
            return client.send_001();
        }
    }
	_send_to_client_channels(client, ":" + old_nickname + " NICK " + client.nickname);
}
