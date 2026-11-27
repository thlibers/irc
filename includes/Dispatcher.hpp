
#pragma once

#include <inttypes.h>
#include <exception>
#include <stdexcept>
#include <vector>
#include <map>

#include "includes/namespace/Replies.hpp"

class User;
class Channel;
class Message;
class Server;

class Dispatcher
{
	private:
		// The server who own this dispatcher: every user, every channel and
		// the connection password are reached through it, and it gives
		// access to QueueMessage to send the numeric replies
		Server	&_server;

		typedef replies::t_return_info (Dispatcher::*Handler)(int, const Message &);

		std::map<std::string, Handler>	_handler;

		// CONNECTION COMMAND HANDLER
		replies::t_return_info	_handlerPass(int fd, const Message &msg);
		replies::t_return_info	_handlerNick(int fd, const Message &msg);
		replies::t_return_info	_handlerUser(int fd, const Message &msg);

		// MESSAGE COMMAND HANDLER
		replies::t_return_info	_handlerPrivmsg(int fd, const Message &msg);

		// KEEP-ALIVE COMMAND HANDLER
		replies::t_return_info	_handlerPing(int fd, const Message &msg);
		replies::t_return_info	_handlerQuit(int fd, const Message &msg);
		replies::t_return_info	_handlerJoin(int fd, const Message &msg);

		Dispatcher::Handler		_getHandler(const std::string &cmd);

	public:
		Dispatcher(Server &server);
		~Dispatcher(void);

		void executeCommand(int user_fd, const char *raw_message);
		void executeCommand(int user_fd, const std::string &raw_message);
};
