
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
		void _parseCommaSeparatedParams(const std::string& token);
	public:
		enum parameter_type
		{
			INVALID = 0,
			USER_NAME = 1,
			CHANNEL_NAME = 2
		};

		Message(void);
		Message(char *raw_str);
		Message(Message &cpy);
		Message	&operator=(Message &cpy);

		void						parse(void);
		void 						parse(const char *raw_message);
		const std::string					getCommand(void) const throw();
		const std::vector<std::string>		getParameter(void) const throw();
		const std::string					getTextMessage(void) const throw();
		const char							*getRawMessage(void) const throw();

		static parameter_type					whichParameterType(std::string &param);
};
