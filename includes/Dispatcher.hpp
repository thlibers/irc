
#pragma once

#include <inttypes.h>
#include <exception>
#include <stdexcept>
#include <vector>
#include <map>

class User;
class Channel;
class Message;
class Server;

class Dispatcher
{
	private:
		// Change to pointer
		std::vector<Channel *>	*_all_channel;
		std::map<int, User *>	*_all_user;
		typedef uint16_t (Dispatcher::*Handler)(int, const Message &);
		std::map<std::string, Handler>	_handler;

		uint16_t	_handle_privmsg(int fd, const Message &msg);

	public:
		Dispatcher(void);
		Dispatcher(std::map<int, User *> *user_map, std::vector<Channel *> *channel_vector);
		Dispatcher(Dispatcher &cpy);
		const Dispatcher &operator=(const Dispatcher &cpy);
		~Dispatcher(void);

		uint16_t executeCommand(int user_fd, const char *raw_message);
};
