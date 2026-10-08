
#pragma once

#include <string>
#include <vector>

class Message
{
	private:
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
		Message(char *raw_str);
		Message(Message &cpy);
		Message	&operator=(Message &cpy);

		void						parse(void);
		std::string					getCommand(void) const throw();
		std::vector<std::string>	getParameter(void) const throw();
		std::string					getTextMessage(void) const throw();
		char						*getRawMessage(void) const throw();

		void						debug(void) const throw();
};
