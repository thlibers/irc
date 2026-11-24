
#include "includes/namespace/helper.hpp"


/*
 * @brief: Function that check if a string contain digits only
 * @return:
 *     - true if the string contain digits only
 *     - false if the string didn't contain digits only
 */
bool	helper::strisdigit(char *str) throw()
{
	for (std::size_t i = 0; str[i]; i++)
	{
		if (!std::isdigit(str[i]))
			return (false);
	}
	return (true);
}
bool	helper::strisdigit(const char *str) throw()
{
	for (std::size_t i = 0; str[i]; i++)
	{
		if (!std::isdigit(str[i]))
			return (false);
	}
	return (true);
}
bool	helper::strisdigit(const std::string &str) throw()
{
	if (str.find_first_not_of("0123456789") != std::string::npos)
		return (false);
	return (true);
}
