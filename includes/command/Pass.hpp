
#pragma once

#include "includes/command/ACommande.hpp"
#include <string>

class User;
class Message;

class Pass : public ACommand
{
	private:
		std::string _new_password;
	public:
		Pass(void);
		Pass(User &user, Message &message);
		Pass(Pass &cpy);
		~Pass(void);

		Pass &operator=(Pass &cpy);

		void	run(void);
};
