
#include "includes/namespace/commands.hpp"
#include "includes/namespace/helper.hpp"
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/irc.hpp"

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// NOT FINISH, NEED TO MERGE WITH channel/Commands

command::Privmsg::Privmsg(::User &user, const ::Message &message, std::map<int, ::User *> &user_map) :
		ACommand(user, message),
		_user_map(user_map)
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
command::Privmsg::Privmsg(Privmsg &cpy) :
		ACommand(cpy),
		_user_map(cpy._user_map),
		_channel(cpy._channel),
		_userlist(cpy._userlist)
{ ; }
command::Privmsg::~Privmsg(void) { ; }

/*
 * @brief: Find a connected user by his nickname, case-insensitively
 * @note:
 *     - RFC 1459 section 1.2: nicknames are case-insensitive, "Abc" and
 *       "abc" are the same user
 * @param: nick, the nickname to look for
 * @param: user_map, the map with every connected user of the server
 * @return: the user found, or NULL if nobody uses this nickname
 */
static ::User	*findUserByNick(const std::string &nick, std::map<int, ::User *> &user_map)
{
	const std::string	upper_nick = helper::strupper(nick);

	for (std::map<int, ::User *>::iterator it = user_map.begin(); it != user_map.end(); ++it)
	{
		if (it->second == NULL)
			continue ;
		if (helper::strupper(it->second->getNickname()) == upper_nick)
			return (it->second);
	}
	return (NULL);
}

replies::t_return_info	command::Privmsg::run(void)
{
	// RFC 1459 section 4.4.1: no target given
	if (this->_channel.empty())
		return (this->_makeReturnInfo(ERR_NORECIPIENT));
	// RFC 1459 section 4.4.1: no text to send (missing or empty ":...")
	if (this->_message.getTextMessage().empty())
		return (this->_makeReturnInfo(ERR_NOTEXTTOSEND));

	// A channel target always starts with a channel prefix (RFC 1459
	// section 1.3), every other target is a nickname
	if (helper::isSet(this->_channel[0], SPECIAL_SET))
	{
		Channel	*channel;

		// A target who starts like a channel but is not a valid channel
		// name is a malformed mask, not an unknown channel
		if (!helper::validChannelName(this->_channel))
			return (this->_makeReturnInfo(ERR_BADCHANMASK, this->_channel));
		// The sender can only talk in a channel he has joined
		channel = this->_user.getChannelByName(this->_channel);
		if (channel == NULL)
			return (this->_makeReturnInfo(ERR_CANNOTSENDTOCHAN, this->_channel));
		// TODO: once the server owns its channel list, split the errors:
		// an unknown channel answers ERR_NOSUCHNICK, a channel where the
		// sender is not a member answers ERR_CANNOTSENDTOCHAN
		// TODO: relay the text message to every member of the channel
		return (this->_makeReturnInfo(0));
	}

	// Nickname target: the message can only be delivered to a user who
	// is connected
	if (findUserByNick(this->_channel, this->_user_map) == NULL)
		return (this->_makeReturnInfo(ERR_NOSUCHNICK, this->_channel));
	// TODO: relay the text message to the target user
	return (this->_makeReturnInfo(0));
}
