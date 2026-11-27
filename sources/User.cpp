
#include "includes/User.hpp"
#include "includes/channel/Channel.hpp"
#include <cstddef>
#include <cstring>
#include <new>
#include <vector>

/*
 * ===============================
 * = USER CONSTRUCTOR/DESTRUCTOR =
 * ===============================
 */

/*
 * @name:  User
 * @brief: The default constructor of our User class, will set every private
 *         attribute to empty
 */
User::User(void) :
	_nickname(""),
	_username(""),
	_real_name(""),
	_fd(-1),
	_alive(true),
	_channel_joined(),
	_registered_flags(User::UNREGISTER)
{ ; }
/*
 * @name: User
 * @brief: The constructor of the User class. This class is the representation
 *         of a data associated to a client such as his username, password, ...
 * @param: nickname, the nickname of the user. This data should be unique
 *         through the entire IRC server
 * @param: username, the username associated of the user
 * @param: real_name, the real name of the user
 * @param: password, the password associated of the user
 */
User::User(const std::string &nickname, const std::string &username, const std::string &real_name) :
	_nickname(nickname),
	_username(username),
	_real_name(real_name),
	_fd(-1),
	_alive(true),
	_channel_joined(),
	_registered_flags(User::UNREGISTER)
{ ; }
User::~User(void) { ; }

/*
 * ===============
 * = USER SETTER =
 * ===============
 */

/*
 * @name: setNickname
 * @brief: Change the new nickname of the user
 * @param: A std::string with the new nickname of the user
 */
void	User::setNickname(const std::string &nickname) throw()
{ this->_nickname = nickname; }
/*
	* @name: setUsername
 * @brief: Change the new username of the user
 * @param: A std::string with the new username of the user
 */
void	User::setUsername(const std::string &username) throw()
{ this->_username = username; }
/*
 * @name: setRealName
 * @brief: Change the new real name of the user
 * @param: A std::string with the new real name of the user
 */
void	User::setRealName(const std::string &realname) throw()
{ this->_real_name = realname; }
/*
 * @name: setFd
 * @brief: Set the fd of the client socket linked to this user
 * @param: The fd of the client socket
 */
void	User::setFd(int fd) throw()
{ this->_fd = fd; }
/*
	* @name: setAlive
	* @brief: Change the alive status of the user, false mean the server has
	*         dropped (or is dropping) his connection
	* @param: The new alive status
	*/
void	User::setAlive(bool alive) throw()
{ this->_alive = alive; }

/*
 * ===============
 * = USER GETTER =
 * ===============
 */

/*
 * @name: getNickname
 * @brief: Return the nickname of the user
 * @return: The nickname of the user
 */
const std::string	&User::getNickname(void) const throw()
{ return (this->_nickname); }
/*
 * @name: getUsername
 * @brief: Return the username of the user
 * @return: The username of the user
 */
const std::string	&User::getUsername(void) const throw()
{ return (this->_username); }
/*
 * @name: getRealName
 * @brief: Return the real name of the user
 * @return: The real name of the user
 */
const std::string	&User::getRealName(void) const throw()
{ return (this->_real_name); }
/*
 * @name: getFd
 * @brief: Return the fd of the client socket linked to this user
 * @return: The fd of the user, -1 if it was never set
 */
int	User::getFd(void) const throw()
{ return (this->_fd); }
/*
	* @name: isAlive
	* @brief: Tell if the user is still connected
	* @return: true while his socket is up, false once the server has
	*          dropped the connection
	*/
bool	User::isAlive(void) const throw()
{ return (this->_alive); }

/*
 * ==========================
 * = USER CHANNEL OPERATION =
 * ==========================
 */

/*
 * @name: isOnChannel
 * @brief: Check if a user is on a channel
 * @param: A reference "channel" to the channel class we want check if the user is on
 * @param: A std::string "channel_name" with the name of the channel we want check if the user is on
 * @return: true if the user is on the channel, false if the user is not on the channel
 */
bool	User::isOnChannel(Channel &channel) const throw()
{
	for (channel_const_it it = this->_channel_joined.begin() ; it != this->_channel_joined.end(); ++it)
	{
		if (*it == &channel)
		{
			if ((*it)->isMember(this->_nickname))
				return (true);
		}
	}
	return (false);
}
bool	User::isOnChannel(std::string &channel_name) const throw()
{
	for (channel_const_it it = this->_channel_joined.begin() ; it != this->_channel_joined.end(); ++it)
	{
		if ((*it)->getName() == channel_name)
		{
			if ((*it)->isMember(this->_nickname))
				return (true);
		}
	}
	return (false);
}
/*
 * @name: addChannel
 * @brief: Add a channel to the channel vector
 * @param: A reference "channel_to_add" to the channel class we want add from our user channel list
 * @return: true if the user has been added to the channel, false if the user is already on the channel
 */
bool	User::addChannel(Channel &channel_to_add) throw()
{
	if (this->isOnChannel(channel_to_add))
		return (false);

	this->_channel_joined.push_back(&channel_to_add);
	return (true);
}
bool	User::addChannel(Channel *channel_to_add) throw()
{
	if (this->isOnChannel(*channel_to_add))
		return (false);

	this->_channel_joined.push_back(channel_to_add);
	return (true);
}
/*
 * @name: removeChannel
 * @brief: Remove a channel to the channel vector, will be usefull when a user will leave a channel
 * @param: A reference "channel" to the channel class we want remove for our user channel list
 * @return: true if the user has been removed from the channel, false if the user is not on the channel
 */
bool	User::removeChannel(Channel &channel_to_remove) throw()
{
	for (channel_it it = _channel_joined.begin() ; it != this->_channel_joined.end(); ++it)
	{
		if (*it == &channel_to_remove)
		{
			this->_channel_joined.erase(it);
			return (true);
		}
	}
	return (false);
}
bool	User::removeChannel(Channel *channel_to_remove) throw()
{
	for (channel_it it = this->_channel_joined.begin() ; it != this->_channel_joined.end() ; ++it)
	{
		if (*it == channel_to_remove)
		{
			this->_channel_joined.erase(it);
			return (true);
		}
	}
	return (false);
}
/*
 * @name: getChannelByName
 * @brief: Get the pointer to a channel from the user channel list by using the name
 *         of the channel
 * @param: An std::string "channel_name" with the name of the channel we want check that the user is on
 * @return: The pointer to the channel class we are searching, NULL if the user is not on the channel
 */
Channel *User::getChannelByName(std::string &channel_name) const throw()
{
	for (channel_const_it it = this->_channel_joined.begin() ; it != this->_channel_joined.end(); ++it)
	{
		if ((*it)->getName() == channel_name)
			return (*it);
	}
	return (NULL);
}

uint8_t	User::isRegisted(void) const throw()
{
	return (this->_registered_flags & REGISTER);
}

/*
 * @name: setFlag
 * @brief: Raise one or more registration flag(s) on the user
 * @param: A uint8_t with the flag(s) to raise, from the e_register_status enum
 */
void	User::setFlag(uint8_t flag) throw()
{ this->_registered_flags |= flag; }

/*
 * @brief: This function is wrapper to easly create a new User
 * @param: nickname, the nickname of the user. This data should be unique throught the entier IRC server
 * @param: username, the username associated of the user
 * @param: real name, the real name of the user
 * @return: Return the new created user or NULL if the user cannot be created
 */
User	*User::createNewUser(std::string &nickname,
							std::string &username,
							std::string &realname)
{
	User *new_user;

	new_user = new(std::nothrow) User(nickname, username, realname);
	return (new_user);
}
