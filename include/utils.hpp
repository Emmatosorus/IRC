#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <vector>
std::string long_to_str(long nbr);
std::string to_irc_lower_case(const std::string& str);
std::vector<std::string> parse_comma_arg(std::string comma_arg);
void remove_unprintable_characters(std::string& str);
void make_unique(std::vector<std::string>& vec);
int sendall(int fd, const std::string& msg);
#endif
