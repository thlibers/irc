
#include "includes/Message.hpp"
#include <cstddef>
#include <cstring>
#include <vector>

char	*strdup(char *tocpy)
{
	char		*str = NULL;
	std::size_t	sz;

	if (strlen(tocpy) == 0)
	{
		sz = 0;
		str = new char[sz + 1];
		str[sz] = '\0';
	}
	else
	{
		sz = std::strlen(tocpy);
		str = new char[sz + 1];
		std::memcpy(str, tocpy, sz + 1);
	}

	return (str);
}

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
			// delete
		this->_raw_message = cpy._raw_message;
	}
	return (*this);
}
Message::Message(char *raw_str) : _command(""), _parameter(), _text_message(""), _raw_message(NULL)
{
	if (this->_raw_message)
		delete []this->_raw_message;
	this->_raw_message = strdup(raw_str);
};
/*
 * @brief: This function is used to parse an IRC line to extract information like
 *         - The command
 *         - The parameter
 *         - The "real" content of the message
 * @throw:
 *     - std::runtime_error with his corresponding error message
 */
void						Message::parse(void)
{
	char	*tok = NULL;

	tok = std::strtok(this->_raw_message, " ");
	while (tok != NULL)
	{

		tok = std::strtok(this->_raw_message, " ");
	}
}

std::string					Message::getCommand(void) const throw() { return (this->_command); }
std::vector<std::string>	Message::getParameter(void) const throw() { return (this->_parameter); }
std::string					Message::getTextMessage(void) const throw() { return (this->_text_message); }
char						*Message::getRawMessage(void) const throw() { return (this->_raw_message); }
