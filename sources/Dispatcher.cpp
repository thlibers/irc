
#include "includes/Dispatcher.hpp"
#include "includes/Message.hpp"

#include <string>
#include <inttypes.h>

uint16_t Dispatcher::executeCommand(User &user, Channel &channel, Message &message)
{
	(void)user;
	(void)channel;
	(void)message;
	std::string wanted_command = message.getCommand();
	if (wanted_command == "PRIVMSG")
		;
	else
		return (421); // NEED TO REPLACE BY A DEFINE
	return (0);
}
