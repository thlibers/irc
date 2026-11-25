
#pragma once

#include <inttypes.h>
#include <exception>
#include <stdexcept>
#include <vector>
#include <map>

class User;
class Channel;
class Message;

class Dispatcher
{
	private:
		// A reference to the vector stored in the Server class how contain every channel
		std::vector<Channel *>	&_all_channel;
		// A reference to the vector stored in the Server class how contain every user
		std::map<int, User *>	&_all_user;
		// The password of the server provided in argv[2]
		std::string 			&_password;

		typedef uint16_t (Dispatcher::*Handler)(int, const Message &);

		std::map<std::string, Handler>	_handler;

		// CONNECTION COMMAND HANDLER
		uint16_t	_handlerPass(int fd, const Message &msg);
		uint16_t	_handlerNick(int fd, const Message &msg);
		// uint16_t	_handlerUser(int fd, const Message &msg);


		// uint16_t	_handle_privmsg(int fd, const Message &msg);

	public:
		Dispatcher(std::map<int, User *> &user_map, std::vector<Channel *> &channel_vector, std::string &password);
		~Dispatcher(void);

		uint16_t executeCommand(int user_fd, const char *raw_message);
		uint16_t executeCommand(int user_fd, const std::string &raw_message);
};
