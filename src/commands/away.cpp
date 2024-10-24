#include "../../include/Server.hpp"

/* https://datatracker.ietf.org/doc/html/rfc1459#section-5.1
 * Parameters: [message] */
void Server::_away(PollfdIterator* it, const std::vector<std::string>& args)
{
	Client& client = m_clients[(*it)->fd];

	if (args.size() == 1)
	{
		client.away_msg = "";
		_send_to_client_channels(client, ":" + client.nickname + " AWAY");
		return client.send_305();
	}
	client.away_msg = args[1];
	_send_to_client_channels(client, ":" + client.nickname + " AWAY :" + args[1]);
	return client.send_306();
}
