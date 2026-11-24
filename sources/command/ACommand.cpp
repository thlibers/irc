
#include "includes/command/ACommande.hpp"
#include <cstddef>

ACommand::ACommand(void) : _user(NULL), _message(NULL) { ; }

ACommand::ACommand(User &user, Message &message) : _user(&user), _message(&message) { ; }

ACommand::~ACommand(void) { ; }

ACommand::ACommand(ACommand &cpy) : _user(cpy._user), _message(cpy._message) { ; }

ACommand &ACommand::operator=(ACommand &cpy)
{
	if (this != &cpy)
	{
		this->_user = cpy._user;
		this->_message = cpy._message;
	}
	return (*this);
}
