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

void Client::send_002()
{
    std::string reply = ":42Chan 002 " + _resolve_nickname() +
                        " :Your host is 42Chan, running version 1.0";
    send_msg(reply);
}

void Client::send_003(const std::string& server_creation_timestamp)
{
    std::string reply = ":42Chan 003 " + _resolve_nickname() + " :This server was created on " +
                        server_creation_timestamp;
    send_msg(reply);
}

void Client::send_004()
{
    std::string reply = ":42Chan 004 " + _resolve_nickname() + " :42chan 1.0 a intokl";
    send_msg(reply);
}

void Client::send_221()
{
    std::string reply = ":42Chan 221 " + _resolve_nickname();
    if (away_msg != "")
        reply += " +a";
    send_msg(reply);
}

void Client::send_301(const std::string& target_nickname, const std::string& target_away_msg)
{
    std::string reply =
        ":42Chan 301 " + _resolve_nickname() + " " + target_nickname + " :" + target_away_msg;
    send_msg(reply);
}

void Client::send_305()
{
    std::string reply =
        ":42Chan 305 " + _resolve_nickname() + " :You are not longer marked as being away";
    send_msg(reply);
}

void Client::send_306()
{
    std::string reply =
        ":42Chan 306 " + _resolve_nickname() + " :You have been marked as being away";
    send_msg(reply);
}

void Client::send_322(const Channel& channel)
{
    std::string reply = ":42Chan 322 " + _resolve_nickname() + " " + channel.name + " " +
                        long_to_str(channel.subscribed_users_fd.size());
    if (channel.topic != "")
        reply += " :" + channel.topic;
    send_msg(reply);
}

void Client::send_323()
{
    std::string reply = ":42Chan 323 " + _resolve_nickname() + " :End of /LIST";
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

void Client::send_372()
{
    std::string reply =
        ":42chan 372 " + _resolve_nickname() + " :Welcome to the 42Chan! :3\n" + ":42chan 372 " +
        _resolve_nickname() + " :  _  _ ___   _____ _                    \n" + ":42chan 372 " +
        _resolve_nickname() + " : | || |__ \\ / ____| |                 \n" + ":42chan 372 " +
        _resolve_nickname() + " : | || |_ ) | |    | |__   __ _ _ __    \n" + ":42chan 372 " +
        _resolve_nickname() + " : |__   _/ /| |    | '_ \\ / _` | '_ \\ \n" + ":42chan 372 " +
        _resolve_nickname() + " :    | |/ /_| |____| | | | (_| | | | |  \n" + ":42chan 372 " +
        _resolve_nickname() + " :    |_|____|\\_____|_| |_|\\__,_|_| |_|\n" + ":42chan 372 " +
        _resolve_nickname() + " :Have fun chatting and be polite to others!";
    send_msg(reply);
}

void Client::send_375()
{
    std::string reply = ":42chan 375 " + _resolve_nickname() + " :- 42Chan Message of the day - ";
    send_msg(reply);
}

void Client::send_376()
{
    std::string reply = ":42chan 376 " + _resolve_nickname() + " :End of /MOTD command.";
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
        ":42chan 461 " + _resolve_nickname() + " " + command + " :Not enough parameters";
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

void Client::send_502()
{
    std::string reply =
        ":42chan 502 " + _resolve_nickname() + " :Can't change mode for other users";
    send_msg(reply);
}

void Client::send_525(const Channel& channel)
{
    std::string reply = ":42chan 525 " + _resolve_nickname() + " " + channel.name +
                        " :Forbidden characters in key: #%:,*?!@. \\t\\r\\n";
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
