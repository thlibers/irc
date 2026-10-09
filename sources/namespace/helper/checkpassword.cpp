
#include "includes/namespace/helper.hpp"
#include "includes/irc.hpp"

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
