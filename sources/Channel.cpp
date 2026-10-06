#include <Channel.hpp>
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

Channel::~Channel(){}

Channel::Channel(Channel &cpy) : 
	_name(cpy._name),
	_topic(cpy._topic),
	_members(cpy._members),
	_operators(cpy._operators),
	_guests(cpy._guests),
	_modes(cpy._modes){
}

Channel &Channel::operator=(Channel &cpy){
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

bool Channel::isMember(std::string member) const
{
	std::vector <std::string>::const_iterator it = this->_members.begin();
	std::vector <std::string>::const_iterator ite = this->_members.end();

	while (it != ite)
	{
		if (member == *it)
			return (true);
		++it;
	}
	return (false);
}