
#pragma once
#include "string"
#include <vector>

class Channel;

class User
{
	private:
		std::string				_name;
		std::string				_real_name;
		std::vector<Channel *>	_channel_list;
	public:
		User(void);
		User(std::string name, std::string real_name);
		User &operator=(User &cpy);
		~User(void);

		void	setName(std::string &name) throw();
		void	setRealName(std::string &real_name) throw();

		bool	isOnChannel(Channel &channel) const throw();
		bool	addChannel(Channel &channel) throw();
		bool	remChannel(Channel &channel) throw();

		std::string getName(void) const throw();
		std::string getRealName(void) const throw();
		Channel *getChannelByName(std::string channel_name) const throw();
};
