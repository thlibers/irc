
#include "includes/namespace/commands.hpp"
#include "includes/namespace/helper.hpp"
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/irc.hpp"

#include <stdexcept>
#include <vector>

command::Privmsg::Privmsg(::User &user, const ::Message &message) : ACommand(user, message)
{
	std::vector<std::string> parameter = this->_message.getParameter();
	std::vector<std::string>::const_iterator it = parameter.begin();
	std::vector<std::string>::const_iterator ite = parameter.end();

	while (it != ite)
	{
		if (this->_channel.empty())
			this->_channel = *it;
		else
			this->_userlist.push_back(*it);
		++it;
	}
}
command::Privmsg::Privmsg(Privmsg &cpy) : ACommand(cpy), _channel(cpy._channel), _userlist(cpy._userlist) { ; }
command::Privmsg::~Privmsg(void) { ; }

uint16_t	command::Privmsg::run(void)
{
	Channel	*channel;

	if (this->_channel.empty())
		return (ERR_NORECIPIENT);
	if (this->_message.getTextMessage().empty())
		return (ERR_NOTEXTTOSEND);
	if (!helper::validChannelName(this->_channel))
		return (ERR_NOSUCHCHANNEL);
	if ((channel = this->_user.getChannelByName(this->_channel)) == NULL)
		return (ERR_CANNOTSENDTOCHAN);

	// SOCKET THINGS GOES HERE
	// The text message is on this->_message.getTextMessage()
	return (0);
}
