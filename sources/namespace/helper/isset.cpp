
#include "includes/namespace/helper.hpp"

/*
 * @brief: This function check if a character is part of a set
 * @return: True if the character is part of the set or False if is not
 */
bool	helper::isSet(std::string character, std::string set) { return (character.find_first_of(set) != std::string::npos); }
bool	helper::isSet(char character, std::string set)
{
	for (std::string::const_iterator i = set.begin() ; i != set.end() ; ++i)
	{
		if (*i == character)
			return (true);
	}
	return (false);
}
