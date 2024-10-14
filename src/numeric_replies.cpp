#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include <string>

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

void Client::send_353(const Channel& channel, const std::string& list_of_clients)
{
	std::string reply = ":42chan 353 " + nickname + " " + channel.name + " : " + list_of_clients;
	send_msg(reply);
}

void Client::send_366(const std::string& channel_name)
{
	std::string reply = ":42chan 366 " + nickname + " " + channel_name + " :End of \\NAMES list";
	send_msg(reply);
}


void Client::send_403(const std::string& channel_name)
{
	std::string reply = ":42chan 403 " + nickname + " " + channel_name + " :No such channel";
	send_msg(reply);
}

void Client::send_442(const Channel& channel)
{
	std::string reply = ":42chan 442 " + nickname + " " + channel.name + " :You're not on that channel";
	send_msg(reply);
}

void Client::send_448(const std::string& channel_name, const std::string& reason)
{
	std::string reply = ":42chan 448 " + nickname + " " + channel_name + " Cannot join channel: " + reason;
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
	std::string reply = ":42chan 482 " + channel.name + " :You're not channel operator";
	send_msg(reply);
}
