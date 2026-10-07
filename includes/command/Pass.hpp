
#pragma once

#include <string>

class User;

class Pass
{
	private:
		User		*_user;
		std::string	_new_password;

	public:
		Pass(void);
		Pass(User *user, std::string new_password);
		Pass(Pass &cpy);
		Pass &operator=(Pass &cpy);
		~Pass(void);
};
