#pragma once

#include <string>
#include <vector>
#include <cstddef>

class User;

typedef struct s_modes {
	bool			inviteOnly;		//	Set/rm Invite-only channel
	bool			topicLocked;	//	Set/rm the restrictions of the TOPIC command to channel ope
	std::string		key;			//	Set/remove the channel key (password), vide = pas de key
	size_t			limit;			//	Set/remove the user limit to channel, 0 = pas de limite

	s_modes() : inviteOnly(false), topicLocked(false), key(), limit(0) {}
}	t_modes;

class Channel
{
private :
	std::string					_name;
	std::string					_topic;
	std::vector	<User *>		_members;
	std::vector	<User *>		_operators;		//	Toujours un sous-ensemble de _members
	std::vector	<User *>		_guests;		//	Invites par INVITE, retires quand ils rejoignent
	t_modes						_modes;

public :
	Channel();
	Channel(const std::string &name, User *creator);
	~Channel();
	Channel(const Channel &cpy);
	Channel &operator=(const Channel &cpy);

	std::string 				getName() const;
	std::string 				getTopic() const;
	const std::vector <User *> 	&getMember() const;
	const std::vector <User *> 	&getOperator() const;
	const std::vector <User *> 	&getGuest() const;
	t_modes 					getMode() const;
	User 						*getMemberByNick(const std::string &nick) const;

	bool isMember(const std::string &nick) const;
	bool isMember(const User *mem) const;
	bool isOperator(const User *ope) const;
	bool isGuest(const User *guest) const;

	void addMember(User *mem);
	void addOperator(User *ope);
	void addGuest(User *guest);
	void rmMember(const User *mem);
	void rmOperator(const User *ope);
	void rmGuest(const User *guest);

	void setTopic(const std::string &topic);
	void setInviteOnly(bool invonly);
	void setTopicLocked(bool lock);
	void setKey(const std::string &key);
	void setLimit(size_t limit);

	size_t membersNb() const;
	bool isChannelEmpty() const;

};

typedef std::vector<Channel*>::iterator			channel_it;
typedef std::vector<Channel*>::const_iterator	channel_const_it;
