
#include "includes/namespace/commands.hpp"
#include <cstddef>

command::ACommand::ACommand(::User &user, const ::Message &message) : _user(user), _message(message) { ; }

command::ACommand::~ACommand(void) { ; }

/*
 * @brief: Build a return info filled with the fresh identity of the sender
 *         at the exact moment of the return
 * @param: code, the IRC code to answer, 0 = nothing to send
 * @param: target, the faulty nick / channel when there is one
 * @return: a t_return_info ready to be given to replies::build
 */
replies::t_return_info	command::ACommand::_makeReturnInfo(uint16_t code, const std::string &target) const
{
	replies::t_return_info	rtn_i(this->_user, this->_message);

	// The constructor already filled nick, user, host, command, fd and the
	// registration snapshot: only the code and the target are left
	rtn_i.exit_code = code;
	rtn_i.target = target;
	return (rtn_i);
}
