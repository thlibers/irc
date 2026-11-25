
#include "includes/namespace/commands.hpp"
#include "includes/Logging.hpp"
#include "includes/User.hpp"
#include "includes/Message.hpp"
#include "includes/irc.hpp"

#include <cstddef>
#include <string>
#include <vector>

command::Pass::Pass(::User &user, const ::Message &message, std::string &server_password) :
		ACommand(user, message),
		_server_password(server_password)
{ ; }

command::Pass::~Pass(void) { ; }

uint16_t	command::Pass::run(void)
{
	std::vector<std::string>	params = this->_message.getParameter();

	// Check if the user have send empty parameter
	if (params.empty() || params[0].empty())
		return (ERR_NEEDMOREPARAMS);

	// If the user have typed the wrong password. The extra parameters
	// of the PASS command are ignored: like every IRC command the first
	// parameter is the one who count (RFC 1459 section 4.1.1)
	if (params[0] != this->_server_password)
	{
		#ifdef DEBUG
		// Debug output
		Logging::info() << this->_user.getFd() << " have typed the wrong password : " << params[0];
		#endif
		return (ERR_PASSWDMISMATCH);
	}

	// Set the password flag to 1
	this->_user.setFlag(::User::PASSWD_SUPPLIED);
	#ifdef DEBUG
	// Debug output
	Logging::info() << this->_user.getFd() << " have connected using the password : " << params[0];
	#endif
	this->_user.setFlag(::User::PASSWD_SUPPLIED);
	return (0);
}
