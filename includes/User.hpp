
#pragma once
#include "string"
#include <vector>

class Channel;

class User
{
	private:
		// The nickname of the user
		std::string				_nickname;
		// The username of the user
		std::string				_username;
		// The real name of the user
		std::string				_real_name;
		// The password of the user
		std::string				_password;
		// The list of every channel that the user is on
		std::vector<Channel *>	_channel_list;
	public:
		User(void);
		User(std::string nickname, std::string username, std::string real_name, std::string password);
		User &operator=(User &cpy);
		~User(void);

		// SET USER RELATED DATA
		void	setNickname(std::string &nickname) throw();
		void	setUsername(std::string &username) throw();
		void	setRealName(std::string &realname) throw();
		void	setPassword(std::string &password) throw();

		// USER CHANNEL RELATED DATA
		bool	isOnChannel(Channel &channel) const throw();
		bool	isOnChannel(std::string &channel) const throw();
		bool	addChannel(Channel &channel) throw();
		bool	remChannel(Channel &channel) throw();
		Channel *getChannelByName(std::string &channel_name) const throw();

		// GET USER RELATED DATA
		std::string	getNickname(void) throw();
		std::string	getUsername(void) throw();
		std::string	getRealName(void) throw();
		std::string	getPassword(void) throw();
};
