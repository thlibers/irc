
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
	_password("")
{ ; }
User::User(std::string nickname, std::string username, std::string real_name, std::string password) :
	_nickname(nickname),
	_username(username),
	_real_name(real_name),
	_password(password)
{ ; }
User::User(User &cpy) :
	_nickname(cpy._nickname),
	_username(cpy._username),
	_real_name(cpy._real_name),
	_password(cpy._password)
{ ; }
User::~User(void)
{
	// Safely clean every password on memory
	if (!this->_password.empty())
		std::memset(&this->_password[0], 0, this->_password.length());
	this->_password.clear();
}

User &User::operator=(User &cpy)
{
	if (this != &cpy)
	{
		this->_nickname = cpy._nickname;
		this->_username = cpy._username;
		this->_real_name = cpy._real_name;
		this->_password = cpy._password;
		this->_channel_list = cpy._channel_list;
	}
	return (*this);
}

/*
 * ======================
 * = USER SETTER/GETTER =
 * ======================
 */

/*
 * @name: setNickname
 * @brief: Change the nickname of the user
 * @param: A std::string with the new nickname of the user
 */
void	User::setNickname(std::string &nickname) throw()
{ this->_nickname = nickname; }
/*
	* @name: setUsername
 * @brief: Change the username of the user
 * @param: A std::string with the new username of the user
 */
void	User::setUsername(std::string &username) throw()
{ this->_username = username; }
/*
 * @name: setRealName
 * @brief: Change the real name of the user
 * @param: A std::string with the new real name of the user
 */
void	User::setRealName(std::string &realname) throw()
{ this->_real_name = realname; }
/*
 * @name: setPassword
 * @brief: Change the password of the user
 * @param: A std::string with the new password of the user
 */
void	User::setPassword(std::string &password) throw()
{ this->_password = password; }
/*
 * @name: getNickname
 * @brief: Get the nickname of the user
 * @return: The nickname of the user
 */
std::string	User::getNickname(void) throw()
{ return (this->_nickname); }
/*
 * @name: getUsername
 * @brief: Get the username of the user
 * @return: The username of the user
 */
std::string	User::getUsername(void) throw()
{ return (this->_username); }
/*
 * @name: getRealName
 * @brief: Get the real name of the user
 * @return: The real name of the user
 */
std::string	User::getRealName(void) throw()
{ return (this->_real_name); }
/*
 * @name: getPassword
 * @brief: Get the password of the user
 * @return: The password of the user
 */
std::string	User::getPassword(void) throw()
{ return (this->_password); }

/*
 * ==========================
 * = USER CHANNEL OPERATION =
 * ==========================
 */

/*
 * @name: isOnChannel
 * @brief: Check if a user is on a channel
 * @param: A reference to the channel class we want check if the user is on
 * @param: A std::string with the name of the channel we want check if the user is on
 * @return: true if the user is on the channel, false if the user is not on the channel
 */
bool	User::isOnChannel(Channel &channel) const throw()
{
	std::vector<Channel*>::const_iterator it = _channel_list.begin();
	std::vector<Channel*>::const_iterator ite = this->_channel_list.end();

	while (it != ite)
	{
		if (*it == &channel)
		{
			if ((*it)->isMember(this->_nickname))
				return (true);
		}
		++it;
	}
	return (false);
}
bool	User::isOnChannel(std::string &channel) const throw()
{
	std::vector<Channel*>::const_iterator it = this->_channel_list.begin();
	std::vector<Channel*>::const_iterator ite = this->_channel_list.end();

	while (it != ite)
	{
		if ((*it)->getName() == channel)
		{
			if ((*it)->isMember(this->_nickname))
				return (true);
		}
		++it;
	}
	return (false);
}
/*
 * @name: addChannel
 * @brief: Add a channel to the channel vector
 * @param: A reference to the channel class we want add from our user channel list
 * @return: true if the user has been added to the channel, false if the user is already on the channel
 */
bool	User::addChannel(Channel &channel) throw()
{
	if (this->isOnChannel(channel))
		return (false);

	this->_channel_list.push_back(&channel);
	return (true);
}
/*
 * @name: removeChannel
 * @brief: Remove a channel to the channel vector, will be usefull when a user will leave a channel
 * @param: A reference to the channel class we want remove for our user channel list
 * @return: true if the user has been removed from the channel, false if the user is not on the channel
 */
bool	User::removeChannel(Channel &channel) throw()
{
	std::vector<Channel*>::iterator	it = _channel_list.begin();
	std::vector<Channel*>::const_iterator ite = _channel_list.end();

	if (!this->isOnChannel(channel))
		goto failed;

	for ( ; it != ite ; ++it)
	{
		if (*it == &channel)
		{
			this->_channel_list.erase(it);
			return (true);
		}
	}
	failed:
		return (false);
}
/*
 * @name: getChannelByName
 * @brief: Get the pointer to a channel from the user channel list by using the name
 *         of the channel
 * @param: The name of the channel we are searching on the joined channel list
 * @return: The pointer to the channel class we are searching, NULL if the user is not on the channel
 */
const Channel *User::getChannelByName(std::string &channel_name) const throw()
{
	std::vector<Channel*>::const_iterator it_beg = this->_channel_list.begin();
	std::vector<Channel*>::const_iterator it_end = this->_channel_list.end();

	while (it_beg != it_end)
	{
		if ((*it_beg)->getName() == channel_name)
			return (*it_beg);
		++it_beg;
	}
	return (NULL);
}
