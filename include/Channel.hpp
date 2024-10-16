#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <ctime>
#include <string>
#include <vector>
class Channel
{
  public:
    Channel(int creator_fd, std::string name);
    Channel(const Channel& c);
    Channel& operator=(const Channel& c);
    ~Channel();

    int creator_fd;
    std::string created_timestamp;
    std::string name;
	std::string topic;
	std::string topic_starter_nickname;
	std::string topic_timestampt;
    std::vector<int> subscribed_users_fd;
    std::vector<int> channel_operators_fd;
    std::vector<int> invited_users_fd;
	std::string password;
	size_t user_limit;
	bool is_password_mode;
	bool is_user_limit_mode;
    bool is_invite_only_mode;
    bool is_const_topic_mode;

	bool is_subscribed(int fd);
	bool is_operator(int fd);
	bool is_invited(int fd);
	void send_msg(const std::string& msg);
	void send_msg_except(int fd, const std::string& msg);
};

#endif
