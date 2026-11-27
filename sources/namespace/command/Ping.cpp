
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/irc.hpp"
#include "includes/namespace/Replies.hpp"
#include "includes/namespace/commands.hpp"

command::Ping::Ping(::User &user, const ::Message &message) : ACommand(user, message)
{ ; }

command::Ping::~Ping(void)
{ ; }


replies::rtn_i_t	command::Ping::run(void)
{
	// The vector with every parameter on it
	const std::vector<std::string>	&params = this->_message.getParameter();
	// The token that the client send
	std::string	token;

	// Token can be put on both first parameter and text message
	if (!params.empty())
		token = params[0];
	else if (!this->_message.getTextMessage().empty())
		token = this->_message.getTextMessage();
	else
		return (replies::rtn_i_t(this->_user, this->_message, ERR_NOORIGIN));

	if (params.size() > 1 && params[1] != SERVER_NAME)
		return (replies::rtn_i_t(this->_user, this->_message, ERR_NOSUCHSERVER));
	return (this->_makeReturnInfo(0, token));
}
