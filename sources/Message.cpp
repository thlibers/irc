

// User includes
#include "includes/Message.hpp"
#include "includes/namespace/helper.hpp"

// Library includes
#include <cstddef>
#include <cstring>
#include <vector>
#include <sstream>
#include <exception>
#include <iostream>

/*
 * @name  Message
 * @brief constructor for C-style string
 * @note  Automaticly parse the line when the list assignement is done
 */
Message::Message(const char *raw_str) : _command(""), _parameter(), _text_message(""), _raw_message(raw_str)
{
	this->parse();
};
/*
 * @name  Message
 * @brief constructor for C++ string
 * @note  Automaticly parse the line when the list assignement is done
 */
Message::Message(const std::string &raw_str) : _command(""), _parameter(), _text_message(""), _raw_message(raw_str)
{
	this->parse();
};
/*
 * @name  Message
 * @brief Destructor
 */
Message::~Message(void) {;}
/*
 * @name  _parseCommaSeparatedParams
 * @param token the
 * @brief
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
 * @name  parse
 * @brief Function that parse the line to get every usefull information like the command
 *        sent, every parameter or the text message
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
/*
 * @name  getCommand
 * @brief getter who return a string with the command
 */
const std::string				&Message::getCommand(void) const throw() { return (this->_command); }
/*
 * @name  getCommand
 * @brief getter who return a vector with every parameter on it
 */
const std::vector<std::string>	&Message::getParameter(void) const throw() { return (this->_parameter); }
/*
 * @name  getCommand
 * @brief getter who return a sttring with the text message on it
 */
const std::string				&Message::getTextMessage(void) const throw() { return (this->_text_message); }
/*
 * @name  getCommand
 * @brief getter who return a string with the non treated string
 */
const std::string				&Message::getRawMessage(void) const throw() { return (this->_raw_message); }
