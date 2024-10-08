#include "../include/Server.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	Server s("", "6667");
	try
	{
		s.start();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}
}
