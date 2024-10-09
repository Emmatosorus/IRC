//
// Created by epolitze on 09/10/24.
//

#include "../include/client_msg_parse.hpp"
static void	split_client_msg(std::string & input, std::vector<std::string> & vec);
static std::string	get_msg(std::string & input);
static void	shrink_space(std::string & input);

std::vector<std::string>	parse_client_msg(std::string input)
{
	std::vector<std::string>	vec;
	std::string					msg;

	msg = get_msg(input);
	split_client_msg(input, vec);
	vec.push_back(msg);
	return (vec);
}

static std::string	get_msg(std::string & input)
{
	size_t	pos = input.find(":", 0);
	std::string	msg;
	if (pos == std::string::npos)
		return msg;
	msg += input.substr(pos + 1, input.size() - pos);
	input.erase(pos, input.size() - pos);
	return msg;
}

static void	split_client_msg(std::string & input, std::vector<std::string> & vec)
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

static void	shrink_space(std::string & input)
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
