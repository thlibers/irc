
#include "includes/User.hpp"
#include "includes/Channel.hpp"
#include <cstddef>
#include <cstring>
#include <vector>

/*
 * ===============================
 * = USER CONSTRUCTOR/DESTRUCTOR =
 * ===============================
 */
User::User(void) :
	_nickname("Anonymous"),
	_username("Anonymous"),
	_real_name("Unknown"),
	_password(""),
	_fd(-1),
	_channel_list()
{ ; }
User::User(std::string nickname, std::string username, std::string real_name, std::string password, int fd) :
	_nickname(nickname),
	_username(username),
	_real_name(real_name),
	_password(password),
	_fd(fd),
	_channel_list()
{ ; }
User::User(User &cpy) :
	_nickname(cpy._nickname),
	_username(cpy._username),
	_real_name(cpy._real_name),
	_password(cpy._password),
	_fd(cpy._fd),
	_channel_list(cpy._channel_list)
{ ; }
const User &User::operator=(User &cpy)
{
	if (this != &cpy)
	{
		this->_nickname		= cpy._nickname;
		this->_username		= cpy._username;
		this->_real_name	= cpy._real_name;
		this->_password		= cpy._password;
		this->_fd			= cpy._fd;
		this->_channel_list	= cpy._channel_list;
	}
	return (*this);
}
User::~User(void)
{
	// Safely clean every password on memory (broken)
	// if (!this->_password.empty())
	// {
	// 	std::memset(&this->_password[0], 0, this->_password.length());
	// 	this->_password.clear();
	// }
}


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
 * @name: setPassword
 * @brief: Change the new password of the user
 * @param: A std::string with the new password of the user
 */
void	User::setPassword(const std::string &password) throw()
{ this->_password = password; }
/*
 * @name: setFd
 * @brief: Change the socket fd associated of the user
 * @param: A int with the new socket fd of the user
 */
void	User::setFd(const int &fd) throw()
{ this->_fd = fd; }

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
const std::string	&User::getNickname(void) throw()
{ return (this->_nickname); }
/*
 * @name: getUsername
 * @brief: Return the username of the user
 * @return: The username of the user
 */
const std::string	&User::getUsername(void) throw()
{ return (this->_username); }
/*
 * @name: getRealName
 * @brief: Return the real name of the user
 * @return: The real name of the user
 */
const std::string	&User::getRealName(void) throw()
{ return (this->_real_name); }
/*
 * @name: getPassword
 * @brief: Return the password of the user
 * @return: The password of the user
 */
const std::string	&User::getPassword(void) throw()
{ return (this->_password); }
/*
 * @name: getFd
 * @brief: Return the socket file descriptor associated of the user
 * @return: The socket number of the user
 */
const int			&User::getFd(void) throw()
{ return (this->_fd); }

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
	for (channel_const_it it = this->_channel_list.begin() ; it != this->_channel_list.end(); ++it)
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
	for (channel_const_it it = this->_channel_list.begin() ; it != this->_channel_list.end(); ++it)
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

	this->_channel_list.push_back(&channel_to_add);
	return (true);
}
bool	User::addChannel(Channel *channel_to_add) throw()
{
	if (this->isOnChannel(*channel_to_add))
		return (false);

	this->_channel_list.push_back(channel_to_add);
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
	for (channel_it it = _channel_list.begin() ; it != this->_channel_list.end(); ++it)
	{
		if (*it == &channel_to_remove)
		{
			this->_channel_list.erase(it);
			return (true);
		}
	}
	return (false);
}
bool	User::removeChannel(Channel *channel_to_remove) throw()
{
	for (channel_it it = this->_channel_list.begin() ; it != this->_channel_list.end() ; ++it)
	{
		if (*it == channel_to_remove)
		{
			this->_channel_list.erase(it);
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
const Channel *User::getChannelByName(std::string &channel_name) const throw()
{
	for (channel_const_it it = this->_channel_list.begin() ; it != this->_channel_list.end(); ++it)
	{
		if ((*it)->getName() == channel_name)
			return (*it);
	}
	return (NULL);
}
