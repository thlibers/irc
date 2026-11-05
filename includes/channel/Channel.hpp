#pragma once

#include <string>
#include <vector>
#include <cstddef>

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
	std::vector	<std::string>	_members;
	std::vector	<std::string>	_operators;		//	Toujours un sous-ensemble de _members
	std::vector	<std::string>	_guests;		//	Invites par INVITE, retires quand ils rejoignent
	t_modes						_modes;

public :
	Channel();
	Channel(const std::string &name, const std::string &creator);
	~Channel();
	Channel(const Channel &cpy);
	Channel &operator=(const Channel &cpy);

	std::string getName() const;
	std::string getTopic() const;
	std::vector <std::string> getMember() const;
	std::vector <std::string> getOperator() const;
	std::vector <std::string> getGuest() const;
	t_modes getMode() const;

	bool isMember(const std::string &mem) const;
	bool isOperator(const std::string &ope) const;
	bool isGuest(const std::string &guest) const;

	void addMember(const std::string &mem);
	void addOperator(const std::string &ope);
	void addGuest(const std::string &guest);
	void rmMember(const std::string &mem);
	void rmOperator(const std::string &ope);
	void rmGuest(const std::string &guest);
	void renameMember(const std::string &oldNick, const std::string &newNick);

	void setTopic(const std::string &topic);
	void setInviteOnly(bool invonly);
	void setTopicLocked(bool lock);
	void setKey(const std::string &key);
	void setLimit(size_t limit);

	size_t membersNb() const;
	bool isChannelEmpty() const;

};
