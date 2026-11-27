
#pragma once

#include <string>
#include <vector>

/*
 * @brief: This class is the representation of the message sent by the user
 * @detail: This class will parse an IRC message to extract information such as
 *     the command, every parameter and the plain text parameter. The sender
 *     is not stored here: the commands receive the User who talked through
 *     the dispatcher, the Message only represents the parsed line.
 *     The raw_message is probably useless but i put it in case
 *     of we need to do the non-parsed string.
 */
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
		const std::string			_raw_message;
		void _parseCommaSeparatedParams(const std::string& token);

	public:
		Message(const char *raw_str);
		Message(const std::string &raw_str);
		~Message();

		void						parse(void);
		const std::string				&getCommand(void) const throw();
		const std::vector<std::string>	&getParameter(void) const throw();
		const std::string				&getTextMessage(void) const throw();
		const std::string				&getRawMessage(void) const throw();

		void						debug(void) const throw();
};
