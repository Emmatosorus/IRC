//
// Created by epolitze on 09/10/24.
//

#include "client_msg_parse.hpp"

int main(int ac, char **av)
{
	std::vector<std::string>	vec;
	if (ac < 2)
		return 0;

	vec = parse_client_msg(av[1]);

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
		else
			std::cout << BOLD << "ARG" << i << " : " << END << *it << "\n";
		i++;
	}

	std::cout << std::endl;
}

std::vector<std::string>	parse_client_msg(std::string input)
{
	std::vector<std::string>	vec;
	std::string					msg;

	msg = get_msg(input);
	split_client_msg(input, vec);
	vec.push_back(msg);
	return (vec);
}

std::string	get_msg(std::string & input)
{
	size_t	pos = input.find(":", 0);
	std::string	msg;
	if (pos == std::string::npos)
		return msg;
	msg += input.substr(pos + 1, input.size() - pos);
	input.erase(pos, input.size() - pos);
	return msg;
}

void	split_client_msg(std::string & input, std::vector<std::string> & vec)
{
	shrink_space(input);
	size_t	i;
	while (!input.empty())
	{
		i = 0;
		while (input[i] != ' ')
			i++;
		vec.push_back(input.substr(0, i));
		input.erase(0, i + 1);
	}
}

void	shrink_space(std::string & input)
{
	size_t	i = 0;
	size_t	j;
	while (input[i])
	{
		i = input.find(' ', i);
		if (i == std::string::npos)
			break;
		if (input[i] == ' ' || input[i] == '\t')
		{
			j = i;
			while (input[j] && (input[j] == ' ' || input[i] == '\t'))
				j++;
			input.erase(i, j - i);
			if (i != 0)
				input.insert(i++, " ");
		}
	}
}