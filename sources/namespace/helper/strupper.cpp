
#include "includes/namespace/helper.hpp"
#include <algorithm>
#include <cctype>

/*
 * @brief: Put a character in uppercase
 * @param: c, the character to get in upper case
 * @return: The upper case version of c
 */
static char	toUpperChar(char c)
{
	return (static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
}

/*
 * @brief: Return an upper case copy of the given string
 * @param: str, the string to fold
 * @return: An upper case copy of str
 */
std::string helper::strupper(const std::string &str)
{
	std::string	upper(str);

	std::transform(upper.begin(), upper.end(), upper.begin(), toUpperChar);
	return (upper);
}
