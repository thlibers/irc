#include "includes/Channel.hpp"
#include "includes/User.hpp"
#include <algorithm>

//	Helpers

static bool contains(const std::vector <User *> &list, const User *user)
{
	return (std::find(list.begin(), list.end(), user) != list.end());
}

static void removeUser(std::vector <User *> &list, const User *user)
{
	user_it it = std::find(list.begin(), list.end(), user);

	if (it != list.end())
		list.erase(it);
}

//	Constructors

Channel::Channel() :
	_name(""),
	_topic(""),
	_members(),
	_operators(),
	_guests(),
	_modes()
{}

//	Le createur d'un channel en est automatiquement operateur
Channel::Channel(const std::string &name, User *creator) :
	_name(name),
	_topic(""),
	_members(),
	_operators(),
	_guests(),
	_modes()
{
	this->addMember(creator);
	this->addOperator(creator);
}

Channel::~Channel(){}

Channel::Channel(const Channel &cpy) :
	_name(cpy._name),
	_topic(cpy._topic),
	_members(cpy._members),
	_operators(cpy._operators),
	_guests(cpy._guests),
	_modes(cpy._modes){
}

Channel &Channel::operator=(const Channel &cpy){
	if (this != &cpy)
	{
		this->_name = cpy._name;
		this->_topic = cpy._topic;
		this->_members = cpy._members;
		this->_operators = cpy._operators;
		this->_guests = cpy._guests;
		this->_modes = cpy._modes;
	}
	return (*this);
}

//	Geters

std::string Channel::getName() const
{
	return (this->_name);
}

std::string Channel::getTopic() const
{
	return (this->_topic);
}

const std::vector <User *> &Channel::getMember() const
{
	return(this->_members);
}

const std::vector <User *> &Channel::getOperator() const
{
	return(this->_operators);
}

const std::vector <User *> &Channel::getGuest() const
{
	return(this->_guests);
}

t_modes Channel::getMode() const
{
	return(this->_modes);
}

User *Channel::getMemberByNick(const std::string &nick) const
{
	for (user_const_it it = this->_members.begin(); it != this->_members.end(); ++it)
	{
		if ((*it)->getNickname() == nick)
			return (*it);
	}
	return NULL;
}


//	Checks

bool Channel::isMember(const std::string &nick) const
{
	return (getMemberByNick(nick) != NULL);
}

bool Channel::isMember(const User *mem) const
{
	return (contains(this->_members, mem));
}

bool Channel::isOperator(const User *ope) const
{
	return (contains(this->_operators, ope));
}

bool Channel::isGuest(const User *guest) const
{
	return (contains(this->_guests, guest));
}

//	Add

//	Rejoindre le channel consomme l'invitation
void Channel::addMember(User *mem)
{
	if (!mem || this->isMember(mem))
		return ;
	this->_members.push_back(mem);
	this->rmGuest(mem);
}

//	Ne fait rien si ope n'est pas membre : MODE +o doit verifier avant pour renvoyer l'erreur 441
void Channel::addOperator(User *ope)
{
	if (!ope || !this->isMember(ope) || this->isOperator(ope))
		return ;
	this->_operators.push_back(ope);
}

void Channel::addGuest(User *guest)
{
	if (guest && !this->isGuest(guest))
		this->_guests.push_back(guest);
}

//	Remove

//	Quitter le channel (PART, KICK, QUIT) fait aussi perdre le statut d'operateur
void Channel::rmMember(const User *mem)
{
	removeUser(this->_members, mem);
	removeUser(this->_operators, mem);
}

void Channel::rmOperator(const User *ope)
{
	removeUser(this->_operators, ope);
}

void Channel::rmGuest(const User *guest)
{
	removeUser(this->_guests, guest);
}

//	Seters

void Channel::setTopic(const std::string &topic)
{
	this->_topic = topic;
}

void Channel::setInviteOnly(bool invonly)
{
	this->_modes.inviteOnly = invonly;
}

void Channel::setTopicLocked(bool lock)
{
	this->_modes.topicLocked = lock;
}

void Channel::setKey(const std::string &key)
{
	this->_modes.key = key;
}

void Channel::setLimit(size_t limit)
{
	this->_modes.limit = limit;
}

//	Utils

size_t Channel::membersNb() const
{
	return (this->_members.size());
}

bool Channel::isChannelEmpty() const
{
	return (this->_members.empty());
}
