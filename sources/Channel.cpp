#include <includes/Channel.hpp>
#include <cstring>
#include <iterator>

Channel::Channel() :
	_name(""),
	_topic(""),
	_members(),
	_operators(),
	_guests(),
	_modes()
{ std::memset(&this->_modes, 0, sizeof(this->_modes)); }

Channel::Channel(std::string name, User &user) : _name(name)
{

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

// Functions

bool Channel::isMember(std::string mem) const
{
	std::vector <std::string>::const_iterator it = this->_members.begin();
	std::vector <std::string>::const_iterator ite = this->_members.end();

	while (it != ite)
	{
		if (mem == *it)
			return (true);
		++it;
	}
	return (false);
}

bool Channel::isOperator(std::string ope) const
{
	std::vector <std::string>::const_iterator it = this->_operators.begin();
	std::vector <std::string>::const_iterator ite = this->_operators.end();

	while (it != ite)
	{
		if (ope == *it)
			return (true);
		++it;
	}
	return (false);
}

bool Channel::isGuest(std::string guest) const
{
	std::vector <std::string>::const_iterator it = this->_guests.begin();
	std::vector <std::string>::const_iterator ite = this->_guests.end();

	while (it != ite)
	{
		if (guest == *it)
			return (true);
		++it;
	}
	return (false);
}

void Channel::addMember(std::string mem)
{
	this->_members.push_back(mem);
}

void Channel::addOperator(std::string ope)
{
	this->_operators.push_back(ope);
}

void Channel::addGuest(std::string guest)
{
	this->_guests.push_back(guest);
}

//	^ Modifs a faire quand la class user sera add.
