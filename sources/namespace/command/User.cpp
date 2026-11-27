#include "includes/Logging.hpp"
#include "includes/namespace/commands.hpp"
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/irc.hpp"
#include "includes/namespace/Replies.hpp"
#include <cstddef>
#include <vector>
#include <bitset>

command::User::User(::User &user, const ::Message &message) :
		ACommand(user, message)
{ ; }

command::User::~User(void) { ; }

static bool	isValidUsername(std::string &username)
{
	// RFC 1459 section 4.1.3: the username becomes the user part of the
	// "<user>@<host>" prefix, it can never contain '@' or a space
	if (username.find_first_of("@ ") != std::string::npos)
		return (false);
	if (username.size() > USERLEN)
		username.resize(USERLEN);
	return (true);
}

replies::t_return_info	command::User::run(void)
{
	std::vector<std::string> params = _message.getParameter();
	std::size_t				 params_size = params.size();

	// If the user is already register, send a already register error
	if ((_user.isRegisted() & _user.USR_SUPPLIED) == _user.USR_SUPPLIED)
		return (this->_makeReturnInfo(ERR_ALREADYREGISTRED));
	// USER needs at least 3 parameters before the realname (RFC 1459
	// section 4.1.3). It also protects the loop below: with less than 3
	// parameters "params_size - 1" would underflow and read out of bounds
	if (params_size < 3)
		return (this->_makeReturnInfo(ERR_NEEDMOREPARAMS));
	// Check that every parameter is not empty
	for (uint8_t i = 0 ; i < params_size - 1 ; i++) // - 1 to accept empty realname
	{
		if (params[i].empty())
			return (this->_makeReturnInfo(ERR_NEEDMOREPARAMS));
	}

	std::string			username = params[0];
	std::string			realname;

	// If the user doesnt give enought parameter, if the
	if (params_size == 3 && !_message.getTextMessage().empty())
		realname = _message.getTextMessage();      // Case with ':' ; USER username 0 * :realname
	else if (params_size == 4 && _message.getTextMessage().empty())
		realname = params[3];                      // Case without ':' ; USER username 0 * realname
	else
		return (this->_makeReturnInfo(ERR_NEEDMOREPARAMS));

	// SILENT FOR NOW NEED TO ENHANCE
	if (!isValidUsername(username))
		return (this->_makeReturnInfo(ERR_INVALIDUSERNAME));
	// Truncate the realname if is too long
	if (realname.size() > REALLEN)
		realname.resize(REALLEN);

	// Set the new username and realname
	this->_user.setUsername(username);
	this->_user.setRealName(realname);
	#ifdef DEBUG
	Logging::info() << "Client " << _user.getFd() << " have set is username to " << username;
	Logging::info() << "Client " << _user.getFd() << " have set is realname to " << realname;
	#endif

	// Set the USER flag to 1
	this->_user.setFlag(::User::USR_SUPPLIED);
	#ifdef DEBUG
	Logging::info() << "Client " << this->_user.getFd() << " bits states " << std::bitset<3>(this->_user.isRegisted()).to_string();
	#endif

	return (this->_makeReturnInfo(0));
}
