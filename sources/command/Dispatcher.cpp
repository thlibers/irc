
#include "includes/command/Dispatcher.hpp"
#include "includes/Message.hpp"
#include "includes/namespace/helper.hpp"
#include "includes/irc.hpp"

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
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "JOIN")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "USER")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "NICK")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "PASS")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "QUIT")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "PING")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "TOPIC")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "INVITE")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "PONG")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "NOTICE")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "KICK")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "PASS")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "MODE")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "JOIN")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "PART")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else if (wanted_command == "NAMES")
		return (ERR_UNKNOWNCOMMAND); // FOR NoW
	else
		return (ERR_UNKNOWNCOMMAND);
}
