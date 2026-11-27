
#include "includes/namespace/commands.hpp"
#include "includes/namespace/Replies.hpp"
#include "includes/Logging.hpp"
#include "includes/User.hpp"
#include "includes/Message.hpp"
#include "includes/irc.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <bitset>

command::Pass::Pass(::User &user, const ::Message &message, std::string &server_password) :
		ACommand(user, message),
		_server_password(server_password)
{ ; }

command::Pass::~Pass(void) { ; }

replies::t_return_info	command::Pass::run(void)
{
	std::vector<std::string>	params = this->_message.getParameter();
	replies::rtn_i_t			info;

	// If a client is already registered
	if ((_user.isRegisted() & ::User::REGISTER) == ::User::REGISTER)
		return (this->_makeReturnInfo(ERR_ALREADYREGISTRED));

	// Check if the user have send empty parameter
	if (params.empty() || params[0].empty())
		return (this->_makeReturnInfo(ERR_NEEDMOREPARAMS));

	// If the user have typed the wrong password. The extra parameters
	// of the PASS command are ignored: like every IRC command the first
	// parameter is the one who count (RFC 1459 section 4.1.1)
	if (params[0] != this->_server_password)
	{
		#ifdef DEBUG
		// Debug output
		Logging::info() << this->_user.getFd() << " have typed the wrong password : " << params[0];
		#endif
		return (this->_makeReturnInfo(ERR_PASSWDMISMATCH));
	}

	// Debug output
	#ifdef DEBUG
	Logging::info() << this->_user.getFd() << " have connected using the password : " << params[0];
	#endif

	info = this->_makeReturnInfo(0);
	// Set the password flag to 1
	this->_user.setFlag(::User::PASSWD_SUPPLIED);
	#ifdef DEBUG
	Logging::info() << "Client " << this->_user.getFd() << " bits states " << std::bitset<3>(this->_user.isRegisted()).to_string();
	#endif

	return (info);
}
