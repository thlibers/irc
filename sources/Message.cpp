
#include "includes/Message.hpp"
#include "includes/helper.hpp"
#include <cstddef>
#include <cstring>
#include <vector>
#include <sstream>
#include <exception>

Message::Message(void) : _command(""), _parameter(), _text_message("") { ; }
Message::Message(Message &cpy) : _command(cpy._command), _parameter(cpy._parameter), _text_message("") { ; };
Message	&Message::operator=(Message &cpy)
{
	if (this != &cpy)
	{
		this->_command = cpy._command;
		this->_parameter = cpy._parameter;
		this->_text_message = cpy._text_message;
		if (this->_raw_message != NULL)
			delete[] this->_raw_message;
		this->_raw_message = cpy._raw_message;
	}
	return (*this);
}
Message::Message(char *raw_str) : _command(""), _parameter(), _text_message(""), _raw_message(NULL)
{
	if (this->_raw_message)
		delete []this->_raw_message;
	this->_raw_message = helper::strdup(raw_str);
	this->parse();
};
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
 * @throw:
 *     - std::runtime_error with his corresponding error message
 * @note:
 *     - Parameters can be comma-separated (e.g., "#ch1,#ch2")
 *     - Trailing text (after ":") is kept as-is
 *     - Example: "PRIVMSG #channel1,#channel2 :Hello world"
 */
void Message::parse(void)
{
	std::istringstream	stream(this->_raw_message);
	std::string			token;

	if (!(stream >> token))
		throw std::runtime_error("Empty message");
	this->_command = token;

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



std::string					Message::getCommand(void) const throw() { return (this->_command); }
std::vector<std::string>	Message::getParameter(void) const throw() { return (this->_parameter); }
std::string					Message::getTextMessage(void) const throw() { return (this->_text_message); }
char						*Message::getRawMessage(void) const throw() { return (this->_raw_message); }
