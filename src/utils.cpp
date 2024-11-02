#include <algorithm>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <vector>

std::string long_to_str(long nbr)
{
    std::stringstream ss;
    ss << nbr;
    return ss.str();
}

std::string to_irc_lower_case(const std::string& str)
{
    std::string res("");
    res.reserve(str.size());
    std::string scandinavian_lower_case_chars = "{}|^";
    std::string scandinavian_upper_case_chars = "[]\\~";
    for (size_t i = 0; i < str.size(); i++)
    {
        size_t scandinavian_char = scandinavian_upper_case_chars.find_first_of(str[i]);
        if (scandinavian_char != std::string::npos)
            res += scandinavian_lower_case_chars[scandinavian_char];
        else
            res += tolower(str[i]);
    }
    return res;
}

std::vector<std::string> parse_comma_arg(std::string comma_arg)
{
    std::vector<std::string> targets;
    size_t pos = 0;
    std::string target;
    while ((pos = comma_arg.find(',')) != std::string::npos)
    {
        target = comma_arg.substr(0, pos);
        targets.push_back(target);
        comma_arg.erase(0, pos + 1);
    }
    targets.push_back(comma_arg);
    return targets;
}

void make_unique(std::vector<std::string>& vec)
{
    std::sort(vec.begin(), vec.end());
    std::vector<std::string>::iterator last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
}

void remove_unprintable_characters(std::string& str)
{
    for (std::string::iterator it = str.begin(); it != str.end();)
    {
        if (!isprint(*it))
            it = str.erase(it);
        else
            it++;
    }
}

int sendall(int fd, const std::string& msg)
{
    int size = msg.size();
    int total = 0;
    int bytes_send = 0;
    while (total < size)
    {
        bytes_send = send(fd, msg.c_str(), size, 0);
        if (bytes_send == -1)
            return -1;
        total += bytes_send;
    }
    return total;
}
