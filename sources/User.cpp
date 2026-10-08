
#include "includes/User.hpp"
#include <cstddef>
#include <vector>

User::User(void) : _nickname("Anonymous"), _username("Anonymous"), _real_name("Unknown"), _password("") { ; }
User::User(std::string nickname, std::string username, std::string real_name, std::string password) : _nickname(nickname), _username(username), _real_name(real_name), _password(password) { ; }
User::~User(void) {;}

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
 * @brief: Change the nickname of the user
 */
void	User::setNickname(std::string &nickname) throw() { this->_nickname = nickname; }
/*
 * @brief: Change the username of the user
 */
void	User::setUsername(std::string &username) throw() { this->_username = username; }
/*
 * @brief: Change the real name of the user
 */
void	User::setRealName(std::string &realname) throw() { this->_real_name = realname; }
/*
 * @brief: Change the password of the user
 */
void	User::setPassword(std::string &password) throw() { this->_password = password; }
/*
 * @brief: Check if a user is on a channel
 * @param:
 *     - channel, a reference to the channel class we want check
 * @return:
 *     - True if the user is on the channel
 *     - False if the user is not on the channel
 */
bool	User::isOnChannel(Channel &channel) const throw()
{
	std::vector<Channel*>::const_iterator it = _channel_list.begin();

	for ( ; it != this->_channel_list.end() ; ++it)
	{
		if (*it == &channel)
			return (true);
	}
	return (false);
}
/*
 * @brief: Add a channel to the channel vector
 * @param:
 *     - channel, a reference to the channel class we want add
 * @return:
 *     - True if the user has been added to the channel
 *     - False if the user is already on the channel
 */
bool	User::addChannel(Channel &channel) throw()
{
	if (this->isOnChannel(channel))
		return (false);

	this->_channel_list.push_back(&channel);
	return (true);
}
/*
 * @brief: Remove a channel to the channel vector
 * @param:
 *     - channel, a reference to the channel class we want add
 * @return:
 *     - True if the user has been removed from the channel
 *     - False if the user is not on the channel
 */
bool	User::remChannel(Channel &channel) throw()
{
	std::vector<Channel*>::const_iterator	it = _channel_list.begin();
	std::size_t								i = 0;

	if (!this->isOnChannel(channel))
		return (false);

	for ( ; it != this->_channel_list.end() ; ++it)
	{
		if (*it == &channel)
			break;
		i++;
	}

	this->_channel_list.erase(this->_channel_list.begin() + i);
	return (true);
}
/*
 * @brief: Return the current username of the user
 */
std::string	User::getName(void) const throw() { return (this->_name); }
/*
 * @brief: Return the current real name of the user
 */
std::string	User::getRealName(void) const throw() { return (this->_real_name); }
/*
 * @brief: Return a pointer to a channel with is name on the user channel list
 */
Channel *User::getChannelByName(std::string channel_name) const throw()
{
	(void)channel_name;
	return (NULL);
}
