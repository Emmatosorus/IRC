//
// Created by epolitze on 07/10/24.
//

#include <iostream>
#include <vector>
#define NB_CMD 10
#define BOLD "\x1B[1m"
#define END "\x1B[0m"

std::vector<std::string>	parse_msg(std::string input);
void	refactor_input(std::string & input);
int	find_cmd(std::string & input, std::vector<std::string> & vec);
void	find_targets(std::string & input, std::vector<std::string> & vec);
void	find_message(std::string & input, std::vector<std::string> & vec);


/*
 * @brief
 * Compile file using :
 * 		c++ parse_msg.cpp -Werror -Wextra -Wall -std=c++98 -o parse
 * Call function with single argument :
 * 		ex: ./parse "PRIVMSG Celia,Eldar : Hey guys!"
 */
int main(int ac, char **av)
{
	std::vector<std::string>	vec;
	if (ac < 2)
		return 0;
	vec = parse_msg(av[1]);
	if (vec.empty())
	{
		std::cerr << "Invalid argument" << std::endl;
		return (1);
	}
	size_t i = 0;
	std::cout << BOLD << "Vector has :\n" << END;
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		if (i == 0)
			std::cout << BOLD << "Command : " << END << *it << "\n";
		else if (i == 1)
			std::cout << BOLD << "Target : " << END << *it << "\n";
		else if (i >= 2)
			std::cout << BOLD << "Message/reste : " << END << *it << std::endl;
		i++;
	}

	std::cout << std::endl;
}

/*
 * @brief Parses an input message and converts it to a command
 * @param input is the message received
 * @return A vector of string containing the parsed command
 */
std::vector<std::string>	parse_msg(std::string input)
{
	std::vector<std::string>	vec;
	refactor_input(input);
	if (find_cmd(input, vec) != 0)
		return (vec);
	find_targets(input, vec);
	find_message(input, vec);
	return (vec);
}

/*
 * @brief shrinks multiple spaces into single spaces inside a string
 * @param input is the string
 */
void	refactor_input(std::string & input)
{
	size_t	i = 0;
	size_t	j;
	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
		{
			j = i;
			while (input[j] && (input[j] == ' ' || input[i] == '\t'))
				j++;
			input.erase(i, j - i);
			if (i != 0)
				input.insert(i++, " ");
			i--;
		}
		else if (input[i] == ',' || input[i] == ':')
		{
			if (i > 0 && input[i - 1] != ' ')
				input.insert(i - 1, " ");
			if (input[i + 1] && input[i + 1] != ' ')
				input.insert(i + 1, " ");
		}
		i++;
	}
}

/*
 * @brief Parses an input message and puts the command at the first place inside the vector
 * @param input is the message received, vec is the vector of strings
 * @return 0 if the command is valid, 1 if it isn't
 */
int	find_cmd(std::string & input, std::vector<std::string> & vec)
{
	std::string	cmds[NB_CMD] = {"USER","NICK","PASS","PRIVMSG","JOIN","LIST","NAMES","NOTICE", "INVITE", "QUIT"};
	int	i = 0;
	size_t	pos;
	while (i < NB_CMD)
	{
		pos = input.find(cmds[i], 0);
		if (pos == std::string::npos)
			i++;
		else if (pos == 0)
		{
			if (input[cmds[i].size()] && input[cmds[i].size()] != ' ')
				return (1);
			vec.push_back(input.substr(0, cmds[i].size()));
			input.erase(0, cmds[i].size() + 1);
			break;
		}
		else
			return (1);
	}
	if (i == NB_CMD)
		return (1);
	return (0);
}

/*
 * @brief Parses an input message and puts targets at the second position of the vector
 * @param input is the message received
 * @param vec is the vector of strings
 * @return 0 if targets were found, 1 if not
 */
void	find_targets(std::string & input, std::vector<std::string> & vec)
{
	std::string	targets;
	size_t j;
	while(input[0])
	{
		j = 0;
		while (input[j] && input[j] != ' ' && input[j] != ',')
			j++;
		targets += input.substr(0, j);
		input.erase(0, j);
		if (input[0] == ' ')
			input.erase(0, 1);
		if (*targets.rbegin() != ',' && input[0] != ',')
			break;
		else
		{
			targets += ',';
			j = 0;
			while (input[j] && (input[j] == ' ' || input[j] == ','))
				j++;
			input.erase(0, j);
		}
	}
	if (targets.empty())
		return ;
	if (*targets.rbegin() == ',')
		targets.erase(targets.size() - 1, 1);
	vec.push_back(targets);
	return ;
}

void	find_message(std::string & input, std::vector<std::string> & vec)
{
	if (input.empty())
		return ;
	size_t i;
	while (input[0])
	{
		if (input[0] == ':' || input[0] == ' ')
		{
			i = 0;
			while (input[i] == ':' || input[i] == ' ')
				i++;
			input.erase(0, i);
		}
		else
		{
			i = 0;
			while (input[i] != ':' && input[i] != ' ')
				i++;
			vec.push_back(input.substr(0, i));
			input.erase(0, i);
		}
	}
	return ;
}