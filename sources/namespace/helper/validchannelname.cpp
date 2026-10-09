
#include "includes/namespace/helper.hpp"

/*
 * @brief: This function if a string is valid channel name
 * @return: True if the character is part of the set or False if is not
 */
bool	helper::validChannelName(std::string &channel)
{
	if (channel.find_first_not_of(UPPER_CASE_SET + LOWER_CASE_SET + DIGIT_SET + SPECIAL_SET) != std::string::npos)
		return (false);
	return (true); // TODO BETTER VERIFICATION, SPECIAL CHARACTER CAN ONLY BE AT THE START OF THE STRING
}
