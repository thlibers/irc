
#pragma once

#include "includes/command/ACommande.hpp"
#include <stdexcept>
#include <string>
#include <vector>

class User;
class Message;
class ACommand;

class Nick : public ACommand
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
