
#include "includes/User.hpp"
#include "includes/Channel.hpp"
#include <new>

/*
 * ===============================
 * = USER CONSTRUCTOR/DESTRUCTOR =
 * ===============================
 */

/*
 * @name: User (default)
 * @brief: Build a placeholder user with no associated connection
 */
User::User(void) :
	_nickname("Anonymous"),
	_username("Anonymous"),
	_real_name("Unknown"),
	_password(""),
	_fd(-1),
	_reg_flags(0),
	_channel_list()
{ ; }
/*
 * @name: User (fd)
 * @brief: Build the user of a freshly accepted connection.
 *     The identity stays empty until NICK/USER are validated and the
 *     registration flags stay unset until the handshake is done.
 * @param: A int "fd" with the socket fd of the new client
 */
User::User(int fd) :
	_nickname(),
	_username(),
	_real_name(),
	_password(""),
	_fd(fd),
	_reg_flags(0),
	_channel_list()
{ ; }
/*
 * @name: User (full)
 * @brief: Build a user with a fully known identity (mostly for tests)
 * @param: The nickname, username, real name, password and socket fd of the user
 */
User::User(const std::string &nickname, const std::string &username,
		   const std::string &real_name, const std::string &password, int fd) :
	_nickname(nickname),
	_username(username),
	_real_name(real_name),
	_password(password),
	_fd(fd),
	_reg_flags(0),
	_channel_list()
{ ; }
/*
 * @name: User (copy)
 * @brief: Copy constructor, shares the same channel pointers as the source
 */
User::User(const User &cpy) :
	_nickname(cpy._nickname),
	_username(cpy._username),
	_real_name(cpy._real_name),
	_password(cpy._password),
	_fd(cpy._fd),
	_reg_flags(cpy._reg_flags),
	_channel_list(cpy._channel_list)
{ ; }
/*
 * @name: operator=
 * @brief: Assignment operator, shares the same channel pointers as the source
 * @param: A const reference "cpy" to the user to copy
 * @return: A reference to the current user
 */
User	&User::operator=(const User &cpy)
{
	if (this != &cpy)
	{
		this->_nickname		= cpy._nickname;
		this->_username		= cpy._username;
		this->_real_name	= cpy._real_name;
		this->_password		= cpy._password;
		this->_fd			= cpy._fd;
		this->_reg_flags	= cpy._reg_flags;
		this->_channel_list	= cpy._channel_list;
	}
	return (*this);
}
User::~User(void)
{
	;
}


/*
 * ===============
 * = USER SETTER =
 * ===============
 */

/*
 * @name: setNickname
 * @brief: Change the nickname of the user
 * @param: A const reference with the new nickname of the user
 */
void	User::setNickname(const std::string &nickname)
{ this->_nickname = nickname; }
/*
 * @name: setUsername
 * @brief: Change the username of the user
 * @param: A const reference with the new username of the user
 */
void	User::setUsername(const std::string &username)
{ this->_username = username; }
/*
 * @name: setRealName
 * @brief: Change the real name of the user
 * @param: A const reference with the new real name of the user
 */
void	User::setRealName(const std::string &realname)
{ this->_real_name = realname; }
/*
 * @name: setPassword
 * @brief: Change the password of the user
 * @param: A const reference with the new password of the user
 */
void	User::setPassword(const std::string &password)
{ this->_password = password; }
/*
 * @name: setFd
 * @brief: Change the socket fd associated with the user
 * @param: A int with the new socket fd of the user
 */
void	User::setFd(int fd)
{ this->_fd = fd; }
/*
 * @name: markRegStep
 * @brief: Mark one step of the registration handshake as validated
 * @param: A t_reg "step" flag (REG_PASS, REG_NICK or REG_USER)
 */
void	User::markRegStep(t_reg step)
{ this->_reg_flags |= (step & REG_ALL); }

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
const std::string	&User::getNickname(void) const
{ return (this->_nickname); }
/*
 * @name: getUsername
 * @brief: Return the username of the user
 * @return: The username of the user
 */
const std::string	&User::getUsername(void) const
{ return (this->_username); }
/*
 * @name: getRealName
 * @brief: Return the real name of the user
 * @return: The real name of the user
 */
const std::string	&User::getRealName(void) const
{ return (this->_real_name); }
/*
 * @name: getPassword
 * @brief: Return the password of the user
 * @return: The password of the user
 */
const std::string	&User::getPassword(void) const
{ return (this->_password); }
/*
 * @name: getFd
 * @brief: Return the socket fd associated with the user
 * @return: The socket fd of the user, -1 if the user has no connection
 */
int	User::getFd(void) const
{ return (this->_fd); }

/*
 * =========================
 * = USER REGISTRATION STATE =
 * =========================
 */

/*
 * @name: isRegistered
 * @brief: Check if the user finished the PASS/NICK/USER registration
 * @return: true if every registration step has been validated,
 *     false otherwise
 */
bool	User::isRegistered(void) const
{ return ((this->_reg_flags & REG_ALL) == REG_ALL); }

/*
 * ==========================
 * = USER CHANNEL OPERATION =
 * ==========================
 */

/*
 * @name: isOnChannel
 * @brief: Check if a user is on a channel, based on the user's own
 *     channel list only (the Channel class keeps its own member list)
 * @param: A const reference "channel" to the channel we want to check
 * @return: true if the channel is in the user's channel list, false otherwise
 */
bool	User::isOnChannel(const Channel &channel) const
{
	for (channel_const_it it = this->_channel_list.begin() ; it != this->_channel_list.end(); ++it)
	{
		if (*it == &channel)
			return (true);
	}
	return (false);
}
/*
 * @name: isOnChannel
 * @brief: Check if a user is on a channel by using its name
 * @param: A const reference "channel_name" with the name of the channel
 * @return: true if the user is on the channel, false otherwise
 */
bool	User::isOnChannel(const std::string &channel_name) const
{
	for (channel_const_it it = this->_channel_list.begin() ; it != this->_channel_list.end(); ++it)
	{
		if ((*it)->getName() == channel_name)
			return (true);
	}
	return (false);
}
/*
 * @name: addChannel
 * @brief: Add a channel to the channel vector
 * @param: A reference "channel_to_add" to the channel we want to add
 *     to the user's channel list
 * @return: true if the channel has been added, false if the user is
 *     already on the channel
 */
bool	User::addChannel(Channel &channel_to_add)
{
	if (this->isOnChannel(channel_to_add))
		return (false);
	this->_channel_list.push_back(&channel_to_add);
	return (true);
}
/*
 * @name: addChannel
 * @brief: Pointer version of addChannel, NULL safe
 * @param: A pointer "channel_to_add" to the channel to add
 * @return: true if the channel has been added, false if the pointer is
 *     NULL or if the user is already on the channel
 */
bool	User::addChannel(Channel *channel_to_add)
{
	if (channel_to_add == NULL)
		return (false);
	return (this->addChannel(*channel_to_add));
}
/*
 * @name: removeChannel
 * @brief: Remove a channel from the channel vector, useful when a user
 *     leaves a channel
 * @param: A const reference "channel_to_remove" to the channel we want
 *     to remove from the user's channel list
 * @return: true if the channel has been removed, false if the user is
 *     not on the channel
 */
bool	User::removeChannel(const Channel &channel_to_remove)
{
	for (channel_it it = this->_channel_list.begin() ; it != this->_channel_list.end() ; ++it)
	{
		if (*it == &channel_to_remove)
		{
			this->_channel_list.erase(it);
			return (true);
		}
	}
	return (false);
}
/*
 * @name: removeChannel
 * @brief: Pointer version of removeChannel, NULL safe
 * @param: A const pointer "channel_to_remove" to the channel to remove
 * @return: true if the channel has been removed, false if the pointer is
 *     NULL or if the user is not on the channel
 */
bool	User::removeChannel(const Channel *channel_to_remove)
{
	if (channel_to_remove == NULL)
		return (false);
	return (this->removeChannel(*channel_to_remove));
}
/*
 * @name: getChannelByName
 * @brief: Get a channel from the user's channel list by using its name
 * @param: A const reference "channel_name" with the name of the channel
 * @return: The pointer to the requested channel, NULL if the user is not
 *     on the channel
 */
Channel	*User::getChannelByName(const std::string &channel_name) const
{
	for (channel_const_it it = this->_channel_list.begin() ; it != this->_channel_list.end() ; ++it)
	{
		if ((*it)->getName() == channel_name)
			return (*it);
	}
	return (NULL);
}

/*
 * @name: createNewUser
 * @brief: Wrapper to easily allocate a new User without ever throwing,
 *     so the server can follow the no-crash rule of the subject
 * @param: nickname, the nickname of the user (unique on the whole server)
 * @param: username, the username of the user
 * @param: realname, the real name of the user
 * @param: password, the password of the user
 * @param: fd, the socket fd of the user's connection
 * @return: The newly created user, or NULL if the allocation failed
 */
User	*User::createNewUser(const std::string &nickname,
							const std::string &username,
							const std::string &realname,
							const std::string &password,
							int fd)
{
	return (new(std::nothrow) User(nickname, username, realname, password, fd));
}
