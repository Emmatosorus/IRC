#include "../include/Channel.hpp"
#include "../include/Client.hpp"
#include "../include/utils.hpp"
#include <string>

void Client::send_001()
{
    std::string reply =
        ":42Chan 001 " + _resolve_nickname() + " :Welcome to the 42Chan Network " + username + "!";
    send_msg(reply);
}

void Client::send_322(const Channel& channel)
{
	std::string reply =
		":42Chan 322 " + _resolve_nickname() + " " + channel.name + " " + long_to_str(channel.subscribed_users_fd.size());
	if (channel.topic != "")
		reply += " :" + channel.topic;
	send_msg(reply);
}

void Client::send_323()
{
    std::string reply =
        ":42Chan 323 " + _resolve_nickname() + " :End of /LIST";
    send_msg(reply);
}

void Client::send_324(const Channel& channel)
{
    std::string reply = ":42Chan 324 " + _resolve_nickname() + " " + channel.name + " ";
	reply += channel.get_modestring();
    send_msg(reply);
}

void Client::send_329(const Channel& channel)
{
    std::string reply =
        ":42Chan 329 " + _resolve_nickname() + " " + channel.name + " " + channel.created_timestamp;
    send_msg(reply);
}

void Client::send_331(const Channel& channel)
{
    std::string reply =
        ":42chan 331 " + _resolve_nickname() + " " + channel.name + " :No topic is set";
    send_msg(reply);
}

void Client::send_332(const Channel& channel)
{
    std::string reply =
        ":42chan 332 " + _resolve_nickname() + " " + channel.name + " :" + channel.topic;
    send_msg(reply);
}

void Client::send_333(const Channel& channel)
{
    std::string reply = ":42chan 333 " + _resolve_nickname() + " " + channel.name + " " +
                        channel.topic_starter_nickname + " " + channel.topic_timestampt;
    send_msg(reply);
}

void Client::send_341(const Channel& channel, const std::string& client_nick)
{
    std::string reply =
        ":42chan 341 " + _resolve_nickname() + " " + client_nick + " " + channel.name;
    send_msg(reply);
}

void Client::send_353(const Channel& channel, const std::string& list_of_clients)
{
    std::string reply =
        ":42chan 353 " + _resolve_nickname() + " = " + channel.name + " :" + list_of_clients;
    send_msg(reply);
}

void Client::send_366(const std::string& channel_name)
{
    std::string reply =
        ":42chan 366 " + _resolve_nickname() + " " + channel_name + " :End of \\NAMES list";
    send_msg(reply);
}

void Client::send_401(const std::string& nonexistent_nickname)
{
    std::string reply = ":42chan 401 " + _resolve_nickname() + " " + nonexistent_nickname +
                        " :No such channel/nickname";
    send_msg(reply);
}

void Client::send_403(const std::string& channel_name)
{
    std::string reply =
        ":42chan 403 " + _resolve_nickname() + " " + channel_name + " :No such channel";
    send_msg(reply);
}

void Client::send_407(const std::string& abort_msg)
{
    std::string reply = ":42chan 407 " + _resolve_nickname() + " :" + abort_msg;
    send_msg(reply);
}

void Client::send_411()
{
    std::string reply = ":42chan 411 " + _resolve_nickname() + ":No recepient";
    send_msg(reply);
}

void Client::send_412()
{
    std::string reply = ":42chan 412 " + _resolve_nickname() + " :No message to send";
    send_msg(reply);
}

void Client::send_417()
{
    std::string reply = ":42chan 417 " + _resolve_nickname() + " :Message is too long";
    send_msg(reply);
}

void Client::send_421(const std::string& command)
{
    std::string reply = ":42chan 421 " + _resolve_nickname() + " " + command + " :Unknown command";
    send_msg(reply);
}

void Client::send_431()
{
    std::string reply = ":42chan 431 " + _resolve_nickname() + +" :No nickname was given";
    send_msg(reply);
}

void Client::send_432(const std::string& new_nickname, const std::string& abort_message)
{
    std::string reply = ":42chan 432 " + _resolve_nickname() + " " + new_nickname +
                        " :Invalid nickname: " + abort_message;
    send_msg(reply);
}

void Client::send_433(const std::string& new_nickname)
{
    std::string reply =
        ":42chan 433 " + _resolve_nickname() + " " + new_nickname + " :Nickname is already taken";
    send_msg(reply);
}

void Client::send_441(const std::string& channel_name)
{
    std::string reply =
        ":42chan 441 " + _resolve_nickname() + " " + channel_name + " :You're not on that channel";
    send_msg(reply);
}

void Client::send_442(const Channel& channel)
{
    std::string reply =
        ":42chan 442 " + _resolve_nickname() + " " + channel.name + " :You're not on that channel";
    send_msg(reply);
}

void Client::send_443(const Channel& channel)
{
    std::string reply =
        ":42chan 443 " + _resolve_nickname() + " " + channel.name + " :User is already on channel";
    send_msg(reply);
}

void Client::send_448(const std::string& channel_name, const std::string& reason)
{
    std::string reply = ":42chan 448 " + _resolve_nickname() + " " + channel_name +
                        " Cannot join channel: " + reason;
    send_msg(reply);
}

void Client::send_451()
{
    std::string reply = ":42chan 451 " + _resolve_nickname() + " :You are not registered";
    send_msg(reply);
}

void Client::send_461(const std::string& command)
{
    std::string reply =
        ":42chan 461 " + _resolve_nickname() + " " + command + " :Not enough prameters";
    send_msg(reply);
}

void Client::send_462()
{
    std::string reply = ":42chan 462 " + _resolve_nickname() + " :Already registered";
    send_msg(reply);
}

void Client::send_464()
{
    std::string reply =
        ":42chan 464 " + _resolve_nickname() + " :Password is incorrect or was not provided";
    send_msg(reply);
}

void Client::send_468(const std::string& abort_msg)
{
    std::string reply = ":42chan 468 " + _resolve_nickname() + " :Invalid username: " + abort_msg;
    send_msg(reply);
}

void Client::send_471(const Channel& channel)
{
    std::string reply =
        ":42chan 471 " + _resolve_nickname() + " " + channel.name + " :Cannot join channel (+l)";
    send_msg(reply);
}

void Client::send_472(char modechar)
{
    std::string reply =
        "42Chan 472 " + _resolve_nickname() + " " + modechar + " :is unknown mode char to me";
    send_msg(reply);
}

void Client::send_473(const Channel& channel)
{
    std::string reply =
        ":42chan 473 " + _resolve_nickname() + " " + channel.name + " :Cannot join channel (+i)";
    send_msg(reply);
}

void Client::send_475(const Channel& channel)
{
    std::string reply =
        ":42chan 475 " + _resolve_nickname() + " " + channel.name + " :Cannot join channel (+k)";
    send_msg(reply);
}

void Client::send_482(const Channel& channel)
{
    std::string reply =
        ":42chan 482 " + _resolve_nickname() + " " + channel.name + " :You're not channel operator";
    send_msg(reply);
}

void Client::send_696(std::string target_name, char modechar, std::string parameter,
                      std::string description)
{
    if (parameter != "")
        parameter = " " + parameter;
    std::string reply = ":42chan 696 " + _resolve_nickname() + " " + target_name + " " + modechar +
                        parameter + " :" + description;
    send_msg(reply);
}
