
#pragma once
#include <string>
#include <vector>

class Channel;
typedef std::vector<Channel *>::const_iterator channel_const_it;
typedef std::vector<Channel *>::iterator channel_it;

/*
 * @brief: This class will contain every user related data
 * @detail: On this class we will store generic data such as the username,
 *     the nickname, the real name and the but password. But also a vector
 *     "Channel" class pointer who represent every channel that the user have
 *     joined.
 */
class User
{
	private:
		// --- ATTRIBUTES ---

		// The nickname of the user (should be unique)
		std::string				_nickname;
		// The username of the user
		std::string				_username;
		// The real name of the user
		std::string				_real_name;
		// The password of the user
		std::string				_password;
		// A vector with every channel that the user is on
		std::vector<Channel *>	_channel_list;

	public:
		// --- CONSTRUCTOR / DESTRUCTOR ---

		User(void);
		User(const std::string &nickname, const std::string &username, const std::string &real_name, const std::string &password);
		User(User &cpy);
		const User &operator=(User &cpy);
		~User(void);

		// --- SETTER ---

		void	setNickname(const std::string &nickname) throw();
		void	setUsername(const std::string &username) throw();
		void	setRealName(const std::string &realname) throw();
		void	setPassword(const std::string &password) throw();

		// --- GETTER ---

		const std::string	&getNickname(void) throw();
		const std::string	&getUsername(void) throw();
		const std::string	&getRealName(void) throw();
		const std::string	&getPassword(void) throw();

		// --- USER CHANNEL ---

		bool	isOnChannel(Channel &channel) const throw();
		bool	isOnChannel(std::string &channel) const throw();
		bool	addChannel(Channel &channel) throw();
		bool	addChannel(Channel *channel) throw();
		bool	removeChannel(Channel &channel) throw();
		bool	removeChannel(Channel *channel) throw();
		Channel *getChannelByName(std::string &channel_name) const throw();

		static User	*createNewUser(std::string &nickname,
								   std::string &username,
								   std::string &realname,
								   std::string &password);
};

typedef std::vector<User*>::iterator		user_it;
typedef std::vector<User*>::const_iterator	user_const_it;
