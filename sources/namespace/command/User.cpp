#include "includes/Logging.hpp"
#include "includes/namespace/commands.hpp"
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/irc.hpp"
#include <vector>

command::User::User(::User &user, const ::Message &message) :
		ACommand(user, message)
{ ; }

command::User::~User(void) { ; }

// static bool	isValidUsername(std::string &username)
// {
// 	(void)username;
// 	return (true);
// }
// static bool	isValidRealname(const std::string realname)
// {
// 	(void)realname;
//
// 	return (true);
// }

uint16_t	command::User::run(void)
{
	std::vector<std::string> params = _message.getParameter();

	if ((_user.isRegisted() & _user.USR_SUPPLIED) == _user.USR_SUPPLIED)
		return (ERR_ALREADYREGISTRED);
	if (params.size() != 3 && _message.getTextMessage().empty())
		return (ERR_NEEDMOREPARAMS);

	std::string			&username = params[0];
	std::string 		&hostname = params[1];
	std::string 		&server_name = params[2];
	const std::string	&realname = _message.getTextMessage();

	// SILENT FOR NOW NEED TO ENHANCE
	(void)username;
	(void)hostname;
	(void)server_name;
	(void)realname;

	this->_user.setRealName(realname);
	this->_user.setUsername(username);
	#ifdef DEBUG
	Logging::info() << "Client " << _user.getFd() << " have set is username to " << username;
	Logging::info() << "Client " << _user.getFd() << " have set is realname to " << realname;
	#endif
	this->_user.setFlag(::User::USR_SUPPLIED);
	return (0);
}
