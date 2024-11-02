#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <ctime>
#include <map>
#include <string>
#include <vector>

class Client;

class Channel
{
  public:
    Channel();
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
    bool is_no_external_messages_mode;

    bool is_subscribed(int fd) const;
    bool is_operator(int fd) const;
    bool is_invited(int fd) const;
    void send_msg(const std::string& msg) const;
    void send_msg_to_operators(int fd, const std::string& msg) const;
    void send_msg_except(int fd, const std::string& msg) const;
    void remove_client(Client& client);
    std::string get_list_of_clients(const std::map<int, Client> m_clients) const;
    std::string get_modestring() const;
};

#endif
