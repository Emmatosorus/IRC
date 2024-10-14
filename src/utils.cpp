#include <string>
#include <sstream>
#include <vector>

std::string long_to_str(long nbr)
{
	std::stringstream ss;
	ss << nbr;
	return ss.str();
}

std::vector<std::string> get_all_targets(std::string targets_str)
{
    std::vector<std::string> targets;
    size_t pos = 0;
    std::string target;
    while ((pos = targets_str.find(',')) != std::string::npos) {
        target = targets_str.substr(0, pos);
        targets.push_back(target);
        targets_str.erase(0, pos + 1);
    }
    targets.push_back(targets_str);
	return targets;
}
