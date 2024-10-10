//
// Created by epolitze on 09/10/24.
//

#include "../include/client_msg_parse.hpp"
#include <algorithm>

static void	split_client_msg(std::string & input, std::vector<std::string> & vec);
static std::string	get_msg(std::string & input);
static void	shrink_space(std::string & input);
static char char_lower(char& c);
static std::string remove_commas(std::string & s);

std::vector<std::string>	parse_client_msg(std::string input)
{
	std::vector<std::string>	vec;
	std::string					msg;

	msg = get_msg(input);
	split_client_msg(input, vec);
	vec.push_back(msg); 
	std::for_each(vec[0].begin(), vec[0].end(), char_lower);
	std::for_each(vec.begin(), vec.end(), remove_commas);
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
		if (it->empty())
			vec.erase(it--);
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
		while (input[i] && input[i] != ' ')
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

static char char_lower(char& c)
{
	c = std::tolower(c);
	return c;
}

static std::string remove_commas(std::string & s)
{
	size_t pos = s.find(',');
	if (pos == std::string::npos)
		return s;
	if (s == ",")
		s.clear();
	else if (pos == 0 || pos == s.size() - 1)
		s.erase(pos, 1);
	return s;
}