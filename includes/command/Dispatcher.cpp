
#include "includes/Dispatcher.hpp"
#include "includes/Message.hpp"
#include "includes/Error.hpp"

#include <string>
#include <inttypes.h>
#include <vector>

uint16_t Dispatcher::executeCommand(std::vector<User *> vec_user, std::vector<Channel *> vec_channel, Message &message)
{
	(void)vec_user;
	(void)vec_channel;
	(void)message;
	std::string wanted_command = message.getCommand();
	if (wanted_command == "PRIVMSG")
		return (commandPrivmsg(vec_user, message));
	else if (wanted_command == "JOIN")
		;
	else if (wanted_command == "USER")
		;
	else if (wanted_command == "NICK")
		;
	else if (wanted_command == "PASS")
		;
	else
		return (ERR_UNKNOWNCOMMAND); // NEED TO REPLACE BY A DEFINE
	return (RPL_SUCCESS);
}

uint16_t	Dispatcher::commandPrivmsg(std::vector<User *> vec_user, Message &message)
{
	(void)vec_user;
	std::vector<std::string> parameter = message.getParameter();
	std::vector<std::string> user_list; // The list of user that we want send the message on the channel
	std::vector<std::string>::const_iterator it = parameter.begin();
	std::vector<std::string>::const_iterator ite = parameter.end();

	while (it != ite)
	{

	}
	return (RPL_SUCCESS);
}
