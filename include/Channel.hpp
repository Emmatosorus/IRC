#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <string>
#include <vector>
class Channel
{
	public:
	Channel(int creator_fd, std::string name);
	~Channel();

	typedef struct s_topic
	{
		std::string topic;
		std::string topic_start_nickname;
		time_t timestampt;
	} t_topic;

	int creator_fd;
	std::string name;
	t_topic topic;
	std::vector<int> subscribed_users_fd;
	std::vector<int> channel_operators_fd;
	std::pair<bool, std::string> password_mode;
	std::pair<bool, size_t> user_limit_mode;
	bool is_invite_only_mode;
	bool is_const_topic_mode;

	private:
	Channel(const Channel& c);
	Channel& operator=(const Channel& c);

};
#endif
