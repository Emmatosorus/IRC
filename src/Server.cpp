#include "../include/Server.hpp"
#include "../include/client_msg_parse.hpp"
#include <csignal>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

bool Server::s_should_run = true;

Server::Server(const char* password, const char* port)
    : m_password(password), m_port(port), m_pfds(), m_sockfd(-1)
{
    _init_listening_socket();

    m_pfds.reserve(MAX_CONNECTIONS);
    struct pollfd serv_pfd;
    serv_pfd.fd = m_sockfd;
    serv_pfd.events = POLLIN;
    serv_pfd.revents = 0;
    m_pfds.push_back(serv_pfd);

    /* Commands are case insensitive, hence they are lower case. */
    m_commands.insert(make_pair("join", &Server::_join));
    m_commands.insert(make_pair("names", &Server::_names));
    m_commands.insert(make_pair("nick", &Server::_nick));
    m_commands.insert(make_pair("notice", &Server::_notice));
    m_commands.insert(make_pair("pass", &Server::_pass));
    m_commands.insert(make_pair("ping", &Server::_ping));
    m_commands.insert(make_pair("pong", &Server::_pong));
    m_commands.insert(make_pair("privmsg", &Server::_privmsg));
    m_commands.insert(make_pair("quit", &Server::_quit));
    m_commands.insert(make_pair("user", &Server::_user));
    m_commands.insert(make_pair("invite", &Server::_invite));
    m_commands.insert(make_pair("topic", &Server::_topic));

    signal(SIGINT, Server::_handle_signal);
}

Server::~Server()
{
    close(m_sockfd);
    for (size_t i = 1; i < m_pfds.size(); i++)
        close(m_pfds[i].fd);
}

void Server::start()
{
    while (Server::s_should_run)
    {
        int poll_res = poll(m_pfds.data(), m_pfds.size(), POLL_TIMEOUT);
        if (poll_res == -1)
            throw std::runtime_error("Critical error: poll failed");

        for (PollfdIterator it = m_pfds.begin(); it != m_pfds.end(); it++)
        {
            if ((it->revents & POLLIN) && it->fd == m_sockfd)
                _handle_client_connection();
            else if ((it->revents & POLLIN) && it->fd != m_sockfd)
                _handle_client_message(it);
        }
    }
}

void Server::_handle_client_connection()
{
    struct sockaddr_storage client_addr;
    socklen_t addrlen = sizeof(struct sockaddr_storage);
    int new_fd = accept(m_sockfd, (struct sockaddr*)&client_addr, &addrlen);

    if (new_fd == -1)
    {
        std::cerr << "Error: new connection failed\n";
        return;
    }

    _add_client(new_fd);
    std::cout << "Connection accepted: " << new_fd << "\n";
    _send_to_client(new_fd, "", "You are connected");
}

void Server::_handle_client_message(PollfdIterator it)
{
    char buf[MESSAGE_SIZE + 1];
    int bytes_received = recv(it->fd, buf, MESSAGE_SIZE, 0);
    if (bytes_received < 0)
    {
        std::cerr << "Error: recv failed\n";
        return;
    }
    // the client is disconnected
    else if (bytes_received == 0)
    {
        std::cout << "Connection closed: " << it->fd << "\n";
        _remove_client(&it);
        return;
    }
    buf[bytes_received] = '\0';
    Client& client = m_clients[it->fd];
    client.buf += buf;
    if (client.buf.length() > MESSAGE_SIZE)
    {
        // TODO: send numeric reply ERR_INPUTTOOLONG (417)
        return;
    }

    size_t end_of_msg = client.buf.find("\r\n");
    std::string raw_message;
    if (end_of_msg != std::string::npos)
    {
        raw_message = client.buf.substr(0, end_of_msg);
        client.buf.clear();
    }
    else
        return;

    std::vector<std::string> parsed_command = parse_client_msg(raw_message);
    const std::string& command = parsed_command[0];
    // TODO: check if the client is registered, forbid everything except PASS
    if (m_commands.find(command) != m_commands.end())
        (this->*m_commands[command])(it, parsed_command);
    else
    {
        // TODO: send numeric reply ERR_UNKNOWNCOMMAND (421)
    }
}

void Server::_init_listening_socket()
{
    struct addrinfo* ai;
    struct addrinfo hints = {};

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int gai_status = getaddrinfo(NULL, m_port.c_str(), &hints, &ai);
    if (gai_status != 0)
        throw std::runtime_error("Critical error: getaddrinfo failed");
    struct addrinfo* curr;
    for (curr = ai; curr != NULL; curr = curr->ai_next)
    {
        m_sockfd = socket(curr->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (m_sockfd < 0)
            continue;

        int yes = 1;
        if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) != 0)
        {
            close(m_sockfd);
            continue;
        }

        if (bind(m_sockfd, curr->ai_addr, curr->ai_addrlen) != 0)
        {
            close(m_sockfd);
            continue;
        }

        break;
    }
    freeaddrinfo(ai);
    if (curr == NULL)
        throw std::runtime_error("Critical error: socket binding failed");

    if (listen(m_sockfd, MAX_CONNECTIONS) != 0)
    {
        close(m_sockfd);
        throw std::runtime_error("Critical error: listen failed");
    }
}

void Server::_remove_client(PollfdIterator* it)
{
    PollfdIterator old_it = *it;
    close(old_it->fd);
    m_clients.erase(old_it->fd);
    *it = m_pfds.erase(old_it);
    std::advance(*it, -1);
}

void Server::_add_client(int fd)
{
    struct pollfd new_pollfd;
    new_pollfd.fd = fd;
    new_pollfd.events = POLLIN;
    new_pollfd.revents = 0;
    Client new_client(fd);
    m_clients.insert(std::make_pair(fd, new_client));
    m_pfds.push_back(new_pollfd);
}

void Server::_send_to_client(int fd, std::string error_code, std::string msg)
{
    std::string total = ":42Chan ";
    std::map<int, Client>::iterator client = m_clients.find(fd);

    total += error_code + " " + client->second.nickname + " :" + msg + "\n";
    send(fd, total.c_str(), total.size(), MSG_CONFIRM);
}

Server::ClientIterator Server::_find_client_by_nickname(const std::string& nickname)
{
    ClientIterator it;
    for (it = m_clients.begin(); it != m_clients.end(); it++)
    {
        if (it->second.nickname == nickname)
            return it;
    }
    return it;
}

void Server::_handle_signal(int signum)
{
    if (signum == SIGINT)
        Server::s_should_run = false;
}
