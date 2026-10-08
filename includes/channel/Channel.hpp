#pragma once

#include <string>
#include <vector>
#include <inttypes.h>

typedef struct s_modes {
	bool			inviteOnly;		//	Set/rm Invite-only channel
	bool			topicLocked;	//	Set/rm the restrictions of the TOPIC command to channel ope
	bool			opeprivilege;	//	Give/take channel operator privilege
	std::string		key;			//	Set/remove the channel key (password)
	size_t			limit;			//	Set/remove the user limit to channel
}	t_modes;

class User;

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
	Channel(std::string name, User &user);
	~Channel();
	Channel(const Channel &cpy);
	Channel &operator=(const Channel &cpy);

	std::string getName() const;
	std::string getTopic() const;
	std::vector <std::string> getMember() const;
	std::vector <std::string> getOperator() const;
	std::vector <std::string> getGuest() const;
	t_modes getMode() const;

	bool isMember(const std::string mem) const;
	bool isOperator(const std::string ope) const;
	bool isGuest(const std::string guest) const;
	void addMember(std::string mem);
	void addOperator(std::string ope);
	void addGuest(std::string guest);

};