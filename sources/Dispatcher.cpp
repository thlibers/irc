
#include "includes/User.hpp"
#include "includes/namespace/helper.hpp"
#include "includes/Dispatcher.hpp"
#include "includes/Message.hpp"
#include "includes/irc.hpp"

#include <string>
#include <inttypes.h>
#include <vector>
#include <iostream>

Dispatcher::Dispatcher(void) :
			_all_channel(NULL),
			_all_user(NULL)
{ ; }

Dispatcher::Dispatcher(std::map<int, User *> *user_map, std::vector<Channel *> *channel_vector) :
			_all_channel(channel_vector),
			_all_user(user_map)
{
	_handler["PRIVMSG"] = &Dispatcher::_handle_privmsg;
}

Dispatcher::Dispatcher(Dispatcher &cpy) :
			_all_channel(cpy._all_channel),
			_all_user(cpy._all_user)
{ ; }

const Dispatcher &Dispatcher::operator=(const Dispatcher &cpy)
{
	if (this != &cpy)
	{
		this->_all_channel = cpy._all_channel;
		this->_all_user = cpy._all_user;
	}
	return (*this);
}

Dispatcher::~Dispatcher(void)
{ ; }

static bool isRegistrationCommand(const std::string &cmd)
{
	return (cmd == "PASS"
			|| cmd == "NICK"
			|| cmd == "USER"
			|| cmd == "PING"
			|| cmd == "QUIT");
}

uint16_t	Dispatcher::_handle_privmsg(int fd, const Message &msg)
{
	(void)fd;
	(void)msg;
	std::cout << "Hey" << std::endl;
	return (0);
}

uint16_t Dispatcher::executeCommand(int user_fd, const char *raw_message)
{
	Message msg(this->_all_user->find(user_fd)->second, raw_message);
	std::map<std::string, Handler>::const_iterator it = this->_handler.find(msg.getCommand());
	(void)isRegistrationCommand;
	// TODO: Check if a user is registered
	// if (!isRegistrationCommand(msg.getCommand()))
		// return (ERR_NOTREGISTERED);
	if (it == _handler.end())
		return (ERR_UNKNOWNCOMMAND);
	std::cout << "User : " << (*_all_user)[user_fd]->getNickname() << " want execute " << msg.getCommand() << std::endl;
	return (this->*(it->second))(user_fd, msg);
}
