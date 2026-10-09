
#include "includes/command/Privmsg.hpp"
#include "includes/Dispatcher.hpp"
#include "includes/command/Commande.hpp"
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/helper.hpp"
#include <stdexcept>
#include <vector>

Privmsg::Privmsg(void) : Command(), _channel(""), _userlist() { ; }
Privmsg::Privmsg(User &user, Message &message) : Command(user, message)
{
	std::vector<std::string> parameter = this->_message->getParameter();
	std::vector<std::string>::const_iterator it = parameter.begin();
	std::vector<std::string>::const_iterator ite = parameter.end();

	if (parameter.empty())
		throw (std::runtime_error("No given parameter!"));

	while (it != ite)
	{
		if (this->_channel.empty())
			this->_channel = *it;
		else
			this->_userlist.push_back(*it);
		++it;
	}
}
Privmsg::Privmsg(Privmsg &cpy) : Command(cpy), _channel(cpy._channel), _userlist(cpy._userlist) { ; }
Privmsg::~Privmsg(void) { ; }

void	Privmsg::run(void)
{
	Channel	*channel;
	(void)channel;
	if (this->_channel.empty())
		throw (std::runtime_error("Missing channel to send a message"));
	if (!helper::validChannelName(this->_channel))
		throw (std::runtime_error("Invalid channel name"));
	if ((channel = this->_user->getChannelByName(this->_channel)) == NULL);
		throw (std::runtime_error("Channel not found"));

	// SOCKET THINGS GOES HERE
	// The text message is on this->_message-> getTextMessage()
}
