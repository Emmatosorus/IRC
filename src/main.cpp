#include "../include/Server.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
	(void)argc;
	try
	{
		std::string	password;
		std::string	port = "6667";
		if (argc == 2)
			password = argv[1];
		else if (argc == 3)
		{
			password = argv[2];
			port = argv[1];
		}
		else
			throw std::invalid_argument("ircserv must be called with : ./ircserv <port> <password>");
		Server s(password.c_str(), port.c_str());
		s.start();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}
}
