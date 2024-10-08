#include "../include/Client.hpp"
#include <unistd.h>

Client::Client(int fd) : fd(fd)
{
	return;
}

Client::~Client()
{
	return;
}
