
#include "includes/namespace/Replies.hpp"
#include "includes/namespace/commands.hpp"
#include "includes/Logging.hpp"
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/namespace/helper.hpp"
#include "includes/irc.hpp"

#include <cctype>
#include <map>
#include <stdexcept>
#include <string>
#include <bitset>
#include <vector>

command::Nick::Nick(::User &user, const ::Message &message, const std::map<int, ::User *> &user_map) :
		ACommand(user, message),
		_user_map(user_map)
{ ; }
command::Nick::Nick(Nick &cpy) : ACommand(cpy), _user_map(cpy._user_map) { ; }
command::Nick::~Nick(void) { ; }

/*
 * @brief: Check the validity of a nickname following the RFC 1459 grammar
 *         <nick> ::= <letter> { <letter> | <number> | <special> }
 * @note:
 *     - The first character must be a letter, never a digit or a special
 *     - The maximum length is nine characters (section 1.2 of the RFC)
 *     - The calls to <cctype> are made on an unsigned char: passing a
 *       negative char (possible because IRC is an 8-bit protocol) would
 *       be an undefined behaviour
 * @param: nick, the nickname to check
 * @return: true if the nickname is valid, false if it is not
 */
static bool	isValid(const std::string &nick)
{
	const std::string	specials("-[]\\`^{}");

	if (nick.empty() || nick.size() > 9)
		return (false);
	if (!std::isalpha(static_cast<unsigned char>(nick[0])))
		return (false);
	for (size_t i = 1; i < nick.size(); i++)
	{
		unsigned char	c = static_cast<unsigned char>(nick[i]);

		if (!std::isalnum(c) && specials.find(nick[i]) == std::string::npos)
			return (false);
	}
	return (true);
}

/*
 * @brief: Check if the nickname is not already used by an other connected
 *         user
 * @param: the nickname wanted by the sender
 * @param: The map wuit every connected user of the server
 * @param: The user who have sent the NICK command
 * @return: true if the nickname is free, false if an other user already
 *          have it
 */
static bool	isAvailable(const std::string &nick,
							const std::map<int, User *> &user_map,
							const User &sender)
{
	const std::string	upper_nick = helper::strupper(nick);

	for (std::map<int, User *>::const_iterator it = user_map.begin(); it != user_map.end(); ++it)
	{
		if (it->second == NULL || it->second == &sender)
			continue ;
		if (helper::strupper(it->second->getNickname()) == upper_nick)
			return (false);
	}
	return (true);
}

replies::t_return_info	command::Nick::run(void)
{
	std::vector<std::string>	params = this->_message.getParameter();
	replies::rtn_i_t			info;

	// Check if the user have send empty parameter
	if (params.empty() || params[0].empty())
	{
		#ifdef DEBUG
		Logging::info() << "The client " << this->_user.getFd() << " has enter an empty nickname";
		#endif
		return (this->_makeReturnInfo(ERR_NONICKNAMEGIVEN));
	}
	// Check if the nickname est valide
	if (!isValid(params[0]))
	{
		#ifdef DEBUG
		Logging::info() << "The client " << this->_user.getFd() << " has enter an invalid nickname " << params[0];
		#endif
		return (this->_makeReturnInfo(ERR_ERRONEUSNICKNAME, params[0]));
	}
	// Check if the nickname is available
	if (!isAvailable(params[0], this->_user_map, this->_user))
	{
		#ifdef DEBUG
		Logging::info() << "The client " << this->_user.getFd() << " has try to set an already used password " << params[0];
		#endif
		return (this->_makeReturnInfo(ERR_NICKNAMEINUSE, params[0]));
	}
	// Set the new nickname
	this->_user.setNickname(params[0]);
	#ifdef DEBUG
	Logging::info() << "The client " << this->_user.getFd() << " has set is nickname to " << params[0];
	#endif

	info = this->_makeReturnInfo(0);

	// Set the NICK flag
	this->_user.setFlag(::User::NICK_SUPPLIED);
	#ifdef DEBUG
	Logging::info() << "Client " << this->_user.getFd() << " bits states " << std::bitset<3>(this->_user.isRegisted()).to_string();
	#endif

	return (info);
}
