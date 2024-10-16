#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include "../include/utils.hpp"
#include <string>

void Client::send_001()
{
	std::string reply = ":42Chan 001 " + nickname + " :Welcome to the 42Chan Network " + username + "!";
	send_msg(reply);
}

void Client::send_324(const Channel& channel)
{
    std::string reply = "42Chan 324 " + nickname + " " + channel.name;
    channel.is_invite_only_mode ? reply += " +i" : reply += " -i";
    channel.is_const_topic_mode ? reply += " +t" : reply += " -t";
    channel.is_password_mode ? reply += " +k " + channel.password : reply += " -k";
    channel.is_user_limit_mode ? reply += " +l " + long_to_str(channel.user_limit) : reply += " -l";
    send_msg(reply);
}

void Client::send_329(const Channel& channel)
{
     std::string reply = "42Chan 329 " + nickname + " " + channel.name + " " + channel.created_timestamp;
    send_msg(reply);
}

void Client::send_331(const Channel& channel)
{
	std::string reply = ":42chan 331 " + nickname + " " + channel.name + " :No topic is set";
	send_msg(reply);
}

void Client::send_332(const Channel& channel)
{
	std::string reply = ":42chan 332 " + nickname + " " + channel.name + " :" + channel.topic;
	send_msg(reply);
}

void Client::send_333(const Channel& channel)
{
	std::string reply = ":42chan 333 " + nickname + " " + channel.name + " " + channel.topic_starter_nickname + " " + channel.topic_timestampt;
	send_msg(reply);
}

void Client::send_341(const Channel& channel, const std::string & client_nick)
{
	std::string reply = ":42chan 341 " + nickname + " " + client_nick + " " + channel.name;
	send_msg(reply);
}

void Client::send_353(const Channel& channel, const std::string& list_of_clients)
{
	std::string reply = ":42chan 353 " + nickname + " " + channel.name + " :" + list_of_clients;
	send_msg(reply);
}

void Client::send_366(const std::string& channel_name)
{
	std::string reply = ":42chan 366 " + nickname + " " + channel_name + " :End of \\NAMES list";
	send_msg(reply);
}

void Client::send_401(const std::string& nonexistent_nickname)
{
	std::string reply = ":42chan 401 " + nickname + " " + nonexistent_nickname + " :No such channel/nickname";
	send_msg(reply);
}

void Client::send_403(const std::string& channel_name)
{
	std::string reply = ":42chan 403 " + nickname + " " + channel_name + " :No such channel";
	send_msg(reply);
}

void Client::send_407(const std::string& abort_msg)
{
	std::string reply = ":42chan 407 " + nickname + " :" + abort_msg;
	send_msg(reply);
}

void Client::send_411()
{
	std::string reply = ":42chan 411 " + nickname + ":No recepient";
	send_msg(reply);
}

void Client::send_412()
{
	std::string reply = ":42chan 412 " + nickname + " :No message to send";
	send_msg(reply);
}

void Client::send_417()
{
	std::string reply = ":42chan 417 " + nickname + " :Message is too long";
	send_msg(reply);
}

void Client::send_421(const std::string& command)
{
	std::string reply = ":42chan 421 " + nickname + " " + command + " :Unknown command";
	send_msg(reply);
}

void Client::send_441(const std::string& channel_name)
{
	std::string reply = ":42chan 441 " + nickname + " " + channel_name + " :You're not on that channel";
	send_msg(reply);
}

void Client::send_442(const Channel& channel)
{
	std::string reply = ":42chan 442 " + nickname + " " + channel.name + " :You're not on that channel";
	send_msg(reply);
}

void Client::send_443(const Channel& channel)
{
	std::string reply = ":42chan 443 " + nickname + " " + channel.name + " :User is already on channel";
	send_msg(reply);
}

void Client::send_448(const std::string& channel_name, const std::string& reason)
{
	std::string reply = ":42chan 448 " + nickname + " " + channel_name + " Cannot join channel: " + reason;
	send_msg(reply);
}

void Client::send_451()
{
	std::string reply = ":42chan 451 anon :You are not registered";
	send_msg(reply);
}

void Client::send_461(const std::string& command)
{
	std::string reply = ":42chan 461 " + nickname + " " + command + " :Not enough prameters";
	send_msg(reply);
}

void Client::send_471(const Channel& channel)
{
	std::string reply = ":42chan 471 " + nickname + " " + channel.name + " :Cannot join channel (+l)";
	send_msg(reply);
}

void Client::send_472(char modechar)
{
    std::string reply = "42Chan 472 " + nickname + " " + modechar + " :is unknown mode char to me";
    send_msg(reply);
}

void Client::send_473(const Channel& channel)
{
	std::string reply = ":42chan 473 " + nickname + " " + channel.name + " :Cannot join channel (+i)";
	send_msg(reply);
}

void Client::send_475(const Channel& channel)
{
	std::string reply = ":42chan 475 " + nickname + " " + channel.name + " :Cannot join channel (+k)";
	send_msg(reply);
}

void Client::send_482(const Channel& channel)
{
	std::string reply = ":42chan 482 " + nickname + " " + channel.name + " :You're not channel operator";
	send_msg(reply);
}

void Client::send_696(std::string target_name, char modechar, std::string parameter, std::string description)
{
    std::string reply = ":42chan 696 " + nickname + " " + target_name + " " + modechar + " " + parameter + " :" + description;
    send_msg(reply);
}
