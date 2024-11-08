#include "../../include/client_msg_parse.hpp"

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
