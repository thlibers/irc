
#include "includes/namespace/helper.hpp"
#include "includes/irc.hpp"


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
