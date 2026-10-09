
#pragma once

#include <inttypes.h>
#include <exception>
#include <stdexcept>

class User;
class Channel;
class Message;
class Server;

class Dispatcher
{
	private:
		Dispatcher(void);
		Dispatcher(Channel &curr_channel, User &curr_user);
		Dispatcher(Dispatcher &cpy);
		Dispatcher &operator=(Dispatcher &cpy);

		uint16_t	commandNick(User &user, Message &message);
		uint16_t	commandUser(User &user, Message &message);
		uint16_t	commandPass(User &user, Message &message);
		uint16_t	commandJoin(User &user, Channel &channel, Message &message);
		uint16_t	commandPrivmsg(User &user, Channel &channel, Message &message);
		uint16_t	commandMode(User &user, Channel &channel, Message &message);

	public:
		static uint16_t executeCommand(User &user, Channel &channel, Message &message, Server &server);
};
