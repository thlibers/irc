
#include "includes/Message.hpp"
#include "includes/namespace/helper.hpp"
#include <cstddef>
#include <cstring>
#include <vector>
#include <sstream>
#include <exception>
#include <iostream>

Message::Message(User &user, const char *raw_str) : _user(user),  _command(""), _parameter(), _text_message(""), _raw_message(raw_str)
{
	this->parse();
};
Message::Message(User &user, char *raw_str) : _user(user),  _command(""), _parameter(), _text_message(""), _raw_message(raw_str)
{
	this->parse();
};
Message::Message(User &user, const std::string &raw_str) : _user(user),  _command(""), _parameter(), _text_message(""), _raw_message(raw_str)
{
	this->parse();
};
Message::Message(Message &cpy) : _user(cpy._user), _command(cpy._command), _parameter(cpy._parameter), _text_message(cpy._text_message), _raw_message(cpy._raw_message) { ; };
/*
 * @brief: Helper function to parse comma-separated parameters
 * @param: token - The token potentially containing comma-separated values
 * @note:
 *     - Splits "param1,param2,param3" into separate parameters
 *     - Example: "JOIN #ch1,#ch2,#ch3" creates three parameters
 */
void Message::_parseCommaSeparatedParams(const std::string& token)
{
	std::stringstream	ss(token);
	std::string			param;

	while (std::getline(ss, param, ','))
	{
		if (!param.empty())
			this->_parameter.push_back(param);
	}
}
/*
 * @brief: This function is used to parse an IRC line to extract information like
 *         - The command
 *         - The parameter
 *         - The "real" content of the message
 * @note:
 *     - Parameters can be comma-separated (e.g., "#ch1,#ch2")
 *     - Trailing text (after ":") is kept as-is
 *     - Example: "PRIVMSG #channel1,#channel2 :Hello world"
 */
void Message::parse(void)
{
	std::istringstream	stream(this->_raw_message);
	std::string			token;

	// Get the first token
	stream >> token;

	// The first token is always the command
	this->_command = helper::strupper(token);

	while (stream >> token)
	{
		if (token[0] == ':')
		{
			// Remove ':'
			this->_text_message = token.substr(1);

			std::string remainder;
			std::getline(stream, remainder);
			if (!remainder.empty())
				this->_text_message += remainder;
			break;
		}
		else
		{
			this->_parseCommaSeparatedParams(token);
		}
	}
	this->_text_message = helper::strtrim(this->_text_message);
}

const User						&Message::getUser(void) const throw() { return (this->_user); }
const std::string				&Message::getCommand(void) const throw() { return (this->_command); }
const std::vector<std::string>	&Message::getParameter(void) const throw() { return (this->_parameter); }
const std::string				&Message::getTextMessage(void) const throw() { return (this->_text_message); }
const std::string				&Message::getRawMessage(void) const throw() { return (this->_raw_message); }

void						Message::debug(void) const throw()
{
	std::vector<std::string>::const_iterator it = this->_parameter.begin();
	std::vector<std::string>::const_iterator ite = this->_parameter.end();

	std::cout << "Command: \"" << this->_command << "\"" << std::endl;
	for (std::size_t i = 1; it != ite; i++)
	{
		std::cout << "Parameter n°" << i << ": \"" << *it << "\"" << std::endl;
		++it;
	}
	std::cout << "Text message: \"" << this->_text_message << "\"" << std::endl;
}
