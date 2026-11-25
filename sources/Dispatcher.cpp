
#include "includes/Logging.hpp"
#include "includes/namespace/commands.hpp"
#include "includes/namespace/helper.hpp"
#include "includes/Dispatcher.hpp"
#include "includes/Message.hpp"
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
 * @param: A reference to the map who contain every user/client connected
 * @param: A reference to the vector who contain every channel available
 * @param: A reference to a string who contained the password to connect to the server (provided in argv[2])
 */
Dispatcher::Dispatcher(std::map<int, User *> &user_map, std::vector<Channel *> &channel_vector, std::string &password) :
			_all_channel(channel_vector),
			_all_user(user_map),
			_password(password)
{
	_handler["PASS"] = &Dispatcher::_handlerPass;
	_handler["NICK"] = &Dispatcher::_handlerNick;
	_handler["NICK"] = &Dispatcher::_handlerNick;
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
	return (cmd == "PASS"
			|| cmd == "NICK"
			|| cmd == "USER"
			|| cmd == "PING"
			|| cmd == "QUIT");
}

uint16_t	Dispatcher::_handlerPass(int fd, const Message &msg)
{
	User	*usr = this->_all_user[fd];

	if (usr == NULL)
		return (ERR_NOTREGISTERED);
	if ((usr->isRegisted() & User::PASSWD_SUPPLIED) == User::PASSWD_SUPPLIED)
		return (ERR_ALREADYREGISTRED);

	command::Pass	pass(*usr, msg, _password);
	return (pass.run());
}

uint16_t	Dispatcher::_handlerNick(int fd, const Message &msg)
{
	User	*usr = this->_all_user[fd];

	command::Nick	nick(*usr, msg, this->_all_user);
	return (nick.run());
}

// uint16_t	Dispatcher::_handlerUser(int fd, const Message &msg)
// {
// 	User	*usr = this->_all_user[fd];
//
// 	command::User	command::User(usr, msg);
// 	return (command::User.run());
// }

/*
 * @name:   executeCommand
 * @brief:  This function will execute check if the command sent by the user is valid
 * @param:  The fd of the user who have sent the command
 * @param:  The raw string sent by the user
 * @return: The IRC code return by the executed command
 */
uint16_t Dispatcher::executeCommand(int user_fd, const std::string &raw_message)
{
	std::map<int, User *>::const_iterator user = this->_all_user.find(user_fd);
	std::map<std::string, Handler>::const_iterator it_handler;
	// uint8_t											register_status = (*user).second->isRegisted();
	uint16_t										code;

	// ERR_NOTREGISTERED seem not appropriated for this situation ngl
	// if (user == this->_all_user.end() || user->second == NULL)
	// 	return (ERR_NOTREGISTERED);

	// Parse the recieve line
	Message msg(*user->second, raw_message);

	// If a user is unregistered and try to executed a registered command who only work for registerd user
	if ((*user).second->isRegisted() != User::REGISTER
		&& !isRegistrationCommand(msg.getCommand()))
		return (ERR_NOTREGISTERED);
	it_handler = this->_handler.find(msg.getCommand());

	// If the command sent by the user doesnt exist
	if (it_handler == _handler.end())
		return (ERR_UNKNOWNCOMMAND);

	// Execute the command from his handler and return the code
	code = (this->*(it_handler->second))(user_fd, msg);
	// TODO WELCOME 001
	// if (((*user).second->isRegisted() & register_status) != User::REGISTER)

	#ifdef DEBUG
	Logging::info() << "Client " << (*user).second->getFd() << " bits states " << std::bitset<4>((*user).second->isRegisted()).to_string();
	#endif
	return (code);
}


/*
 * @name:   executeCommand
 * @brief:  This function will execute check if the command sent by the user is valid
 * @param:  The fd of the user who have sent the command
 * @param:  The raw string sent by the user
 * @return: The IRC code return by the executed command
 */
uint16_t Dispatcher::executeCommand(int user_fd, const char *raw_message)
{
	const std::string msg = raw_message;
	return (Dispatcher::executeCommand(user_fd, msg));
}
