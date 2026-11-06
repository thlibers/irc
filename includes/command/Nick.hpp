
#pragma once

#include "includes/command/Commande.hpp"
#include <stdexcept>
#include <string>
#include <vector>

class User;
class Message;
class Command;

class Nick : public Command
{
	private:
		std::string					_new_nickname;
	public:
		Nick(void);
		Nick(User &user, Message &message);
		Nick(Nick &cpy);
		~Nick(void);

		void	run(void);
};
