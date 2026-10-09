
#include "includes/User.hpp"
#include "includes/Channel.hpp"
#include <cstddef>
#include <vector>

/*
 * ===============================
 * = USER CONSTRUCTOR/DESTRUCTOR =
 * ===============================
 */
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
 * ======================
 * = USER SETTER/GETTER =
 * ======================
 */

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
 * @brief: Get the nickname of the user
 */
std::string	User::getNickname(void) throw() { return (this->_nickname); }
/*
 * @brief: Get the username of the user
 */
std::string	User::getUsername(void) throw() { return (this->_username); }
/*
 * @brief: Get the real name of the user
 */
std::string	User::getRealName(void) throw() { return (this->_real_name); }
/*
 * @brief: Get the password of the user
 */
std::string	User::getPassword(void) throw() { return (this->_password); }


/*
 * ==========================
 * = USER CHANNEL OPERATION =
 * ==========================
 */

/*
 * @brief: Check if a user is on a channel
 * @param:
 *     - channel, a reference to the channel class we want check or the name of the channel
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
bool	User::isOnChannel(std::string &channel) const throw()
{
	std::vector<Channel*>::const_iterator it = _channel_list.begin();

	for ( ; it != this->_channel_list.end() ; ++it)
	{
		if ((*it)->getName() == channel)
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
 * @brief: Remove a channel to the channel vector, will be usefull when a user will
 *     leave a channel
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
	Channel									*ptr = NULL;

	if (!this->isOnChannel(channel))
		return (false);

	for ( ; it != this->_channel_list.end() ; ++it)
	{
		if (*it == &channel)
		{
			ptr = *it;
			break;
		}
		i++;
	}
	if (ptr != NULL)
		this->_channel_list.erase(this->_channel_list.begin() + i);
	return (true);
}

/*
 * @brief: Get the pointer to channel from the user channel list by searching using the name
 *     of the channel
 * @param:
 *     - channel_name name of the channel we search
 */
Channel *User::getChannelByName(std::string &channel_name) const throw()
{
	std::vector<Channel *>::const_iterator it_beg = this->_channel_list.begin();
	std::vector<Channel *>::const_iterator it_end = this->_channel_list.end();

	while (it_beg != it_end)
	{
		if ((*it_beg)->getName() == channel_name)
			return (*it_beg);
		++it_beg;
	}
	return (NULL);
}
