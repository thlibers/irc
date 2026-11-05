#include "includes/channel/Channel.hpp"
#include <algorithm>

//	Helpers

static bool contains(const std::vector <std::string> &list, const std::string &nick)
{
	return (std::find(list.begin(), list.end(), nick) != list.end());
}

static void removeNick(std::vector <std::string> &list, const std::string &nick)
{
	std::vector <std::string>::iterator it = std::find(list.begin(), list.end(), nick);

	if (it != list.end())
		list.erase(it);
}

static void renameNick(std::vector <std::string> &list, const std::string &oldNick, const std::string &newNick)
{
	std::replace(list.begin(), list.end(), oldNick, newNick);
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
/* J'ai remplacé Channel(std::string, User &)
par Channel(const std::string &name, const std::string &creator).
User n'existe pas sur cette branche (std::vector<User *> une fois User mergé)*/
Channel::Channel(const std::string &name, const std::string &creator) :
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

std::vector <std::string> Channel::getMember() const
{
	return(this->_members);
}

std::vector <std::string> Channel::getOperator() const
{
	return(this->_operators);
}

std::vector <std::string> Channel::getGuest() const
{
	return(this->_guests);
}

t_modes Channel::getMode() const
{
	return(this->_modes);
}

//	Checks

bool Channel::isMember(const std::string &mem) const
{
	return (contains(this->_members, mem));
}

bool Channel::isOperator(const std::string &ope) const
{
	return (contains(this->_operators, ope));
}

bool Channel::isGuest(const std::string &guest) const
{
	return (contains(this->_guests, guest));
}

//	Add

//	Rejoindre le channel consomme l'invitation
void Channel::addMember(const std::string &mem)
{
	if (this->isMember(mem))
		return ;
	this->_members.push_back(mem);
	this->rmGuest(mem);
}

//	Ne fait rien si ope n'est pas membre : MODE +o doit verifier avant pour renvoyer l'erreur 441
void Channel::addOperator(const std::string &ope)
{
	if (!this->isMember(ope) || this->isOperator(ope))
		return ;
	this->_operators.push_back(ope);
}

void Channel::addGuest(const std::string &guest)
{
	if (!this->isGuest(guest))
		this->_guests.push_back(guest);
}

//	Remove

//	Quitter le channel (PART, KICK, QUIT) fait aussi perdre le statut d'operateur
void Channel::rmMember(const std::string &mem)
{
	removeNick(this->_members, mem);
	removeNick(this->_operators, mem);
}

void Channel::rmOperator(const std::string &ope)
{
	removeNick(this->_operators, ope);
}

void Channel::rmGuest(const std::string &guest)
{
	removeNick(this->_guests, guest);
}

//	A appeler par NICK sur chaque channel du user, sinon il perd sa place et ses droits
void Channel::renameMember(const std::string &oldNick, const std::string &newNick)
{
	renameNick(this->_members, oldNick, newNick);
	renameNick(this->_operators, oldNick, newNick);
	renameNick(this->_guests, oldNick, newNick);
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
