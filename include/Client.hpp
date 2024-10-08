#ifndef CLIENT_HPP
#define CLIENT_HPP
class Client
{
	public:
	Client(int fd);
	Client(const Client& c) = default;
	Client& operator=(const Client& c) = default;
	~Client();
	int fd;
};
#endif
