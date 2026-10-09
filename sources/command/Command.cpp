
#include "includes/command/Commande.hpp"
#include <cstddef>

Command::Command(void) : _user(NULL), _message(NULL) { ; }

Command::Command(User &user, Message &message) : _user(&user), _message(&message) { ; }

Command::~Command(void) { ; }

Command::Command(Command &cpy) : _user(cpy._user), _message(cpy._message) { ; }

Command &Command::operator=(Command &cpy)
{
	if (this != &cpy)
	{
		this->_user = cpy._user;
		this->_message = cpy._message;
	}
	return (*this);
}
