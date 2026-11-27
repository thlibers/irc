
#include "includes/namespace/Replies.hpp"
#include "includes/namespace/commands.hpp"

command::Quit::Quit(::User &user, const ::Message &message) :
	ACommand(user, message)
{ ; }

command::Quit::~Quit(void)
{ ; }

replies::rtn_i_t	command::Quit::run(void)
{
	this->_user.setAlive(false);
	return (replies::rtn_i_t(this->_user, this->_message, 0));
}
