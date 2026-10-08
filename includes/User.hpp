
#pragma once
#include "string"
#include <vector>

class Channel;

class User
{
	private:
		// The nickname of the user
		std::string				_nickname;
		std::string				_username;
		std::string				_real_name;
		std::string				_password;
		std::vector<Channel *>	_channel_list;
	public:
		User(void);
		User(std::string nickname, std::string username, std::string real_name, std::string password);
		User &operator=(User &cpy);
		~User(void);

		void	setNickname(std::string &nickname) throw();
		void	setUsername(std::string &username) throw();
		void	setRealName(std::string &realname) throw();
		void	setPassword(std::string &password) throw();

		bool	isOnChannel(Channel &channel) const throw();
		bool	addChannel(Channel &channel) throw();
		bool	remChannel(Channel &channel) throw();

		std::string getName(void) const throw();
		std::string getRealName(void) const throw();
		Channel *getChannelByName(std::string channel_name) const throw();
};
