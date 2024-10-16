#include <string>
#include <sstream>
#include <vector>

std::string long_to_str(long nbr)
{
	std::stringstream ss;
	ss << nbr;
	return ss.str();
}

std::vector<std::string> parse_comma_arg(std::string comma_arg)
{
    std::vector<std::string> targets;
    size_t pos = 0;
    std::string target;
    while ((pos = comma_arg.find(',')) != std::string::npos) {
        target = comma_arg.substr(0, pos);
        targets.push_back(target);
        comma_arg.erase(0, pos + 1);
    }
    targets.push_back(comma_arg);
	return targets;
}
