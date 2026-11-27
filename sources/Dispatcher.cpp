
#include "includes/namespace/commands.hpp"
#include "includes/namespace/Replies.hpp"
#include "includes/namespace/helper.hpp"
#include "includes/Dispatcher.hpp"
#include "includes/Logging.hpp"
#include "includes/Message.hpp"
#include "includes/Server.hpp"
#include "includes/User.hpp"
#include "includes/irc.hpp"

#include <inttypes.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>

/*
 * @name:  Dispatcher
 * @brief: The constructor for the dispatcher
 * @param: A reference to the server who own this dispatcher: every user,
 *         every channel and the connection password are reached through it
 */
Dispatcher::Dispatcher(Server &server) :
			_server(server)
{
	_handler["PASS"] = &Dispatcher::_handlerPass;
	_handler["NICK"] = &Dispatcher::_handlerNick;
	_handler["USER"] = &Dispatcher::_handlerUser;
	_handler["PING"] = &Dispatcher::_handlerPing;
	_handler["QUIT"] = &Dispatcher::_handlerQuit;
	_handler["JOIN"] = &Dispatcher::_handlerJoin;
	// _handler["PRIVMSG"] = &Dispatcher::_handlerPrivmsg;
}
/*
 * @name:  Dispatcher
 * @brief: The destructor for the dispatcher class (do nothing)
 */
Dispatcher::~Dispatcher(void)
{ ; }
/*
 * @name:   isRegistrationCommand
 * @brief:  This function check if the comment sent by the user can be executed
 *          by a non registered client
 * @param:  The command to check
 * @return: true if it is a non registered command, false if it not
 */
static bool isRegistrationCommand(const std::string &cmd)
{
	return (   cmd == "PASS"
			|| cmd == "NICK"
			|| cmd == "USER"
			|| cmd == "PING"
			|| cmd == "QUIT");
}
/*
 * @name _handlerPass
 * @brief This function is the handler for the PASS command
 * @param The file descriptior associated with the user
 * @param A Message object who contain the line with every information
 */
replies::t_return_info	Dispatcher::_handlerPass(int fd, const Message &msg)
{
	User	*usr = this->_server.getClients()[fd];

	command::Pass	pass(*usr, msg, this->_server.getPassword());
	return (pass.run());
}
/*
 * @name _handlerNick
 * @brief This function is the handler for the NICK command
 * @param The file descriptior associated with the user
 * @param A Message object who contain the line with every information
 */
replies::t_return_info	Dispatcher::_handlerNick(int fd, const Message &msg)
{
	User	*usr = this->_server.getClients()[fd];

	command::Nick	nick(*usr, msg, this->_server.getClients());
	return (nick.run());
}
/*
 * @name _handlerUser
 * @brief This function is the handler for the USER command
 * @param The file descriptior associated with the user
 * @param A Message object who contain the line with every information
 */
replies::t_return_info	Dispatcher::_handlerUser(int fd, const Message &msg)
{
	User	*usr = this->_server.getClients()[fd];

	command::User	user(*usr, msg);
	return (user.run());
}
/*
 * @name _handlerPrivmsg
 * @brief This function is the handler for the PRIVMSG command
 * @param The file descriptior associated with the user
 * @param A Message object who contain the line with every information
 */
replies::t_return_info	Dispatcher::_handlerPrivmsg(int fd, const Message &msg)
{
	User	*usr = this->_server.getClients()[fd];

	command::Privmsg	privmsg(*usr, msg, this->_server.getClients());
	return (privmsg.run());
}

replies::t_return_info	Dispatcher::_handlerPing(int fd, const Message &msg)
{
	User	*usr = this->_server.getClients()[fd];

	command::Ping	ping(*usr, msg);
	replies::t_return_info	rtn_i = ping.run();

	// PONG is the only reply who is not a numeric: it cannot pass through
	// replies::build(). The token came back in the target field, the echo
	// is queued here, where the server is reachable
	if (rtn_i.exit_code == 0)
		this->_server.QueueMessage(fd, err::pong(rtn_i.target));
	return (rtn_i);
}

replies::t_return_info	Dispatcher::_handlerQuit(int fd, const Message &msg)
{
	User	*usr = this->_server.getClients()[fd];

	command::Quit	quit(*usr, msg);
	return (quit.run());
}

replies::t_return_info	Dispatcher::_handlerJoin(int fd, const Message &msg)
{
	User	*usr = this->_server.getClients()[fd];
	(void)msg; // Shut up

	command::Join	join(*usr, msg);
	return (join.run());
}

/*
 * @name: _getHandler
 * @brief: This function will search to the correct handler
 * @param: The command name in string
 */
Dispatcher::Handler	Dispatcher::_getHandler(const std::string &cmd)
{
	std::map<std::string, Handler>::const_iterator	it_handler;

	it_handler = this->_handler.find(cmd);
	if (it_handler == _handler.end())
		return (NULL);
	// idk
	return (it_handler->second);
}
/*
 * @name:   executeCommand
 * @brief:  This function will execute check if the command sent by the user is valid
 * @param:  The fd of the user who have sent the command
 * @param:  The raw string sent by the user
 * @return: The IRC code return by the executed command
 */
void Dispatcher::executeCommand(int user_fd, const std::string &raw_message)
{
	// Get the "User" class of the user using the fd who send a command
	std::map<int, User *>::const_iterator			user = this->_server.getClients().find(user_fd);
	// The pointer to the handler who is the command to execute
	Handler											handler;

	if (!user->second)
		return;

	#ifdef DEBUG
	Logging::info() << "Client " << (*user).second->getFd() << " want to execute " << raw_message;
	#endif

	// Parse the recieve line
	Message msg(raw_message);

	// Append User information and message information on the context struct
	replies::rtn_i_t	rtn_i(*(*user).second, msg, this->_server.getCreateAt());

	// Go search for the correct handler
	if ((handler = _getHandler(msg.getCommand())) == NULL)
	{
		_server.QueueMessage(user_fd, replies::build(ERR_UNKNOWNCOMMAND,rtn_i));
		return;
	}

	// If a user is unregistered and try to executed a registered command who only work for registerd user
	if ((*user).second->isRegisted() != User::REGISTER
		&& !isRegistrationCommand(msg.getCommand()))
	{
		_server.QueueMessage(user_fd, replies::build(ERR_NOTREGISTERED, rtn_i));
		return;
	}

	// Execute the command: the handler returns everything build() needs
	rtn_i = (this->*(handler))(user_fd, msg);

	rtn_i.date = this->_server.getCreateAt();

	// If the user wast register before and is now register, send a 001 message
	if (rtn_i.snapshot_registration != User::REGISTER
		&& (*user).second->isRegisted() == User::REGISTER)
	{
		this->_server.QueueMessage(user_fd, replies::build(RPL_WELCOME, rtn_i));
		this->_server.QueueMessage(user_fd, replies::build(RPL_YOURHOST, rtn_i));
		this->_server.QueueMessage(user_fd, replies::build(RPL_CREATED, rtn_i));
		this->_server.QueueMessage(user_fd, replies::build(RPL_MYINFO, rtn_i));
		this->_server.QueueMessage(user_fd, replies::build(RPL_ISUPPORT, rtn_i));
		this->_server.QueueMessage(user_fd, replies::build(ERR_NOMOTD, rtn_i));
	}
	// If we have a rpl ou err to write (know it by checking the code that return our command)
	else if (rtn_i.exit_code > 0)
	{
		this->_server.QueueMessage(user_fd,
								   replies::build(rtn_i.exit_code, rtn_i));
	}
}
/*
 * @name:   executeCommand
 * @brief:  This function will execute check if the command sent by the user is valid
 * @param:  The fd of the user who have sent the command
 * @param:  The raw string sent by the user
 */
void Dispatcher::executeCommand(int user_fd, const char *raw_message)
{
	const std::string msg = raw_message;
	Dispatcher::executeCommand(user_fd, msg);
}
