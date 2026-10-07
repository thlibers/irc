
#include "includes/command/Pass.hpp"

Pass::Pass(void) : _user(NULL), _new_password("") {;}
Pass::Pass(User *user, std::string new_password) : _user(user), _new_password(new_password) { ; }
Pass::Pass(Pass &cpy) : _user(cpy._user), _new_password(cpy._new_password) { ; }
Pass::~Pass(void) {;}

Pass &Pass::operator=(Pass &cpy)
{
	if (this != &cpy)
	{
		this->_user = cpy._user;
		this->_new_password = cpy._new_password;
	}
	return (*this);
}
