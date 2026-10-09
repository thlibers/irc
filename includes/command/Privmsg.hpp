
#pragma once

#include "includes/command/Commande.hpp"
#include <stdexcept>
#include <string>
#include <vector>

class User;
class Message;
class Command;

class Privmsg : public Command
{
	private:
		std::string					_channel;
		std::vector<std::string>	_userlist;
	public:
		Privmsg(void);
		Privmsg(User &user, Message &message);
		Privmsg(Privmsg &cpy);
		~Privmsg(void);

		void	run(void);

		// class FailedExec : public std::runtime_error
		// {
		// 	private:
		// 		std::string _message;
		// 	public:
		// 		FailedExec(void);
		// 		FailedExec(std::string messasge);
		// 		virtual ~FailedExec(void);
		// 		virtual const char	*what(void) const throw();
		// };
};
