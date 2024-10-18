#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#pass-message
 * Parameters: <password> */
void Server::_pass(PollfdIterator* it, const std::vector<std::string>& args)
{
	Client& client = m_clients[(*it)->fd];
	if (args.size() != 2)
		return client.send_461("PASS");

	if (client.is_registered)
		return client.send_462();

	client.password = args[1];
	client.entered_password = true;
}
