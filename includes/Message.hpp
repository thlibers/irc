
#pragma once

#include <string>
#include <vector>

class User;

/*
 * @brief: This class it the representation of the message sent but the user
 * @detail: This class will parse an IRC message to extract information such as
 *     the command, every parameter and the plain text parameter. I have put a User * to
 *     know easly who sent the message. The raw_message is probably useless but i put it in case
 *     of we need to do the non-parsed string.
 */
class Message
{
	private:
		// The user who send the message
		const User					*_user;
		// The command wanted by the user
		std::string 				_command;
		// The parameter that will succed the command
		std::vector<std::string>	_parameter;
		// The "real" message that the user want send to an channel/user
		std::string 				_text_message;
		// The message string without any treatement
		char						*_raw_message;

	public:
		Message(void);
		Message(User *user, char *raw_str);
		Message(Message &cpy);
		Message	&operator=(Message &cpy);

		void						parse(void);
		User						*getUser(void) const throw();
		std::string					getCommand(void) const throw();
		std::vector<std::string>	getParameter(void) const throw();
		std::string					getTextMessage(void) const throw();
		char						*getRawMessage(void) const throw();

		void						debug(void) const throw();
};
