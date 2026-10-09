
#pragma once

#include <inttypes.h>
#include <exception>
#include <stdexcept>
#include <vector>

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

		static uint16_t	commandNick(User &user, Message &message);
		static uint16_t	commandUser(User &user, Message &message);
		static uint16_t	commandPass(User &user, Message &message);
		static uint16_t	commandJoin(User &user, Channel &channel, Message &message);
		static uint16_t	commandPrivmsg(std::vector<User *> vec_user, Message &message);
		static uint16_t	commandMode(User &user, Channel &channel, Message &message);

	public:
		static uint16_t executeCommand(std::vector<User *> vec_user, std::vector<Channel *> vec_channel, Message &message);
};
