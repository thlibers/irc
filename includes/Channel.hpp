#pragma once

#include <string>
#include <vector>
#include <inttypes.h>

typedef struct s_modes {
	bool			inviteOnly;
	bool			topicLocked;
	std::string		key;
	int8_t			limit;
}	t_modes;

class Channel
{
private :
	std::string					_name;
	std::string					_topic;
	std::vector	<std::string>	_members;
	std::vector	<std::string>	_operators;
	std::vector	<std::string>	_guests;
	t_modes						_modes;	

public :
	Channel();
	~Channel();
	Channel(Channel &cpy);
	Channel &operator=(Channel &cpy);

	std::string getName() const;
	std::string getTopic() const;
	std::vector <std::string> getMember() const;
	std::vector <std::string> getOperator() const;
	std::vector <std::string> getGuest() const;
	t_modes getMode() const;

	bool Channel::isMember(std::string member) const;
	bool Channel::isOperator(std::string member) const;
	bool Channel::isGuest(std::string member) const;

};