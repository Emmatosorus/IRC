//
// Created by epolitze on 09/10/24.
//

#ifndef CLIENT_MSG_PARSE_HPP
#define CLIENT_MSG_PARSE_HPP

#include <iostream>
#include <vector>
#define NB_CMD 10
#define BOLD "\x1B[1m"
#define END "\x1B[0m"

std::vector<std::string> parse_client_msg(std::string input);

#endif
