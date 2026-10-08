
#include "includes/User.hpp"
#include <cstddef>
#include <vector>

User::User(void) : _name("Anonymous"), _real_name("Unknown") { ; }
User::User(std::string name, std::string real_name) : _name(name), _real_name(real_name) { ; }
User::~User(void) {;}

User &User::operator=(User &cpy)
{
	if (this != &cpy)
	{
		this->_name = cpy._name;
		this->_real_name = cpy._real_name;
	}
	return (*this);
}

/*
 * @brief: Change the username of the user
 */
void	User::setName(std::string &name) throw()
{ this->_name = name; }
/*
 * @brief: Change the real name of the user
 */
void	User::setRealName(std::string &real_name) throw()
{ this->_real_name = real_name; }
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
