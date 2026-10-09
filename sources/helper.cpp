
#include "includes/helper.hpp"
#include "includes/Error.hpp"
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <inttypes.h>

/*
 * ==============
 * = STRISDIGIT =
 * ==============
 */

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

/*
 * =============
 * = CHECKPORT =
 * =============
 */

/*
 * @brief: Function that check if a port provided on a string is valid.
 * @return:
 *     - The number of the port to use for the server in a uint16_t type
 * @throw:
 *     - std::runtime_error with the reason of the throw
 */
uint16_t	helper::checkPort(const char *cstr_port)
{
	std::string str_port(cstr_port);

	if (!helper::strisdigit(str_port)
			|| str_port.length() > 5
			|| str_port == "0"
			|| helper::strtoany<int>(cstr_port) > 65535)
		throw (std::runtime_error(EINVALID_PORT));
	return (helper::strtoany<uint16_t>(str_port));
}
uint16_t	helper::checkPort(const std::string &str)
{
	if (!helper::strisdigit(str)
			|| str.length() > 5
			|| str == "0"
			|| helper::strtoany<int>(str) > 65535)
		throw (std::runtime_error(EINVALID_PORT));
	return (helper::strtoany<uint16_t>(str));
}

/*
 * =================
 * = CHECKPASSWORD =
 * =================
 */

/*
 * @brief: Function that check if a provided password is valid.
 * @return:
 *     - The password as a std::string
 * @throw:
 *     - std::runtime_error with the reason of the throw
 */
std::string helper::checkPassword(char *_str)
{
	std::string password(_str);

	if (password.length() > 1024)
		std::runtime_error(EPWD_TOO_LONG);
	return (password);
}
std::string helper::checkPassword(std::string &_str)
{
	if (_str.length() > 1024)
		std::runtime_error(EPWD_TOO_LONG);
	return (_str);
}


/*
 * ===========
 * = STRTRIM =
 * ===========
 */

/*
 * @brief: Function will trim every whitespace on a string and return the
 *     timmed string.
 * @return:
 *     - The string without whitespace at the begining and the end
 */
std::string	helper::strtrim(char *str) throw()
{
	std::string cpy_str;
	std::size_t	size;
	std::size_t first = 0;
	std::size_t last;

	if (!str)
		return ("");

	cpy_str = str;
	size = cpy_str.size();
	last = size;

	while (first < last && std::isspace(static_cast<uint8_t>(cpy_str[first])))
		++first;
	while (last > first && std::isspace(static_cast<uint8_t>(cpy_str[last - 1])))
		--last;

	if (last != size)
		cpy_str.erase(last, size);
	if (first != 0)
		cpy_str.erase(0, first);
	return (cpy_str);
}

std::string	helper::strtrim(std::string &str) throw()
{
	std::string	cpy_str = str;
	std::size_t first = 0;
	std::size_t last;
	std::size_t	size;

	if (cpy_str.empty())
		return ("");

	size = cpy_str.size();
	last = size;

	while (first < last && std::isspace(static_cast<uint8_t>(cpy_str[first])))
		++first;
	while (last > first && std::isspace(static_cast<uint8_t>(cpy_str[last - 1])))
		--last;

	if (last != size)
		cpy_str.erase(last, size);
	if (first != 0)
		cpy_str.erase(0, first);
	return (cpy_str);
}

std::string	helper::strtrim(std::string &str, std::string set) throw()
{
	std::string	cpy_str = str;
	std::size_t first = 0;
	std::size_t last;
	std::size_t	size;

	if (cpy_str.empty())
		return ("");

	size = cpy_str.size();
	last = size;

	cpy_str.find_last_of(set);
	while (first < last && helper::isSet(static_cast<uint8_t>(cpy_str[first]), set))
		++first;
	while (last > first && helper::isSet(static_cast<uint8_t>(cpy_str[last - 1]), set))
		--last;

	if (last != size)
		cpy_str.erase(last, size);
	if (first != 0)
		cpy_str.erase(0, first);
	return (cpy_str);
}
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

static std::string	getLowercase(void)
{
	std::string s;
	char c = 'a';

	while (c <= 'z')
	{ s += c; }
	return (s);
}
static std::string	getUppercase(void)
{
	std::string s;
	char c = 'A';

	while (c <= 'Z')
	{ s += c; }
	return (s);
}
/*
 * @brief: This function if a string is valid channel name
 * @return: True if the character is part of the set or False if is not
 */
bool	helper::validChannelName(std::string &channel)
{
	return (channel.find_first_not_of(getLowercase() + getUppercase() + "#!&+") != std::string::npos);
}
