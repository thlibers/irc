
#include "includes/Message.hpp"
#include "includes/namespace/helper.hpp"
#include <cstddef>
#include <cstring>
#include <vector>
#include <iostream>

/*
 * @brief: Function that allocated and copy a string in a second one without
 *     using malloc like the real function (use new instead)
 * @return:
 *     - The copy of the string to copy
 * @throw:
 *     - std::bad_alloc if a memory allocation failed
 */
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

Message::Message(void) : _user(NULL), _command(""), _parameter(), _text_message("") { ; }
Message::Message(Message &cpy) : _user(cpy._user), _command(cpy._command), _parameter(cpy._parameter), _text_message("") { ; };
Message	&Message::operator=(Message &cpy)
{
	if (this != &cpy)
	{
		this->_user = cpy._user;
		this->_command = cpy._command;
		this->_parameter = cpy._parameter;
		this->_text_message = cpy._text_message;
		if (this->_raw_message != NULL)
			// delete
		this->_raw_message = cpy._raw_message;
	}
	return (*this);
}
Message::Message(User *user, char *raw_str) : _user(user),  _command(""), _parameter(), _text_message(""), _raw_message(NULL)
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
		// The first token is always a command
		if (this->_command.empty())
			this->_command = tok;
		// The text messages always start with a ":"
		else if (!this->_command.empty() && tok[0] == ':')
		{
			// Every token after a ":" will be considered as a text messages
			while(tok != NULL)
			{
				// Temporary solution, manually add only one space between each token
				if (!this->_text_message.empty())
					this->_text_message += " ";
				if (this->_text_message.empty() && tok[0] == ':')
					this->_text_message += &tok[1];
				else
					this->_text_message += tok;
				tok = std::strtok(NULL, " ");
			}
		}
		else
			// After a command, everything is a parameter until the end of the string of
			// we find a token that start with a ":"
			this->_parameter.push_back(tok);
		// Get the next token
		tok = std::strtok(NULL, " ");
	}
	this->_text_message = helper::strtrim(this->_text_message);
}

std::string					Message::getCommand(void) const throw() { return (this->_command); }
std::vector<std::string>	Message::getParameter(void) const throw() { return (this->_parameter); }
std::string					Message::getTextMessage(void) const throw() { return (this->_text_message); }
char						*Message::getRawMessage(void) const throw() { return (this->_raw_message); }

void						Message::debug(void) const throw()
{
	std::vector<std::string>::const_iterator it = this->_parameter.begin();
	std::vector<std::string>::const_iterator itend = this->_parameter.end();

	std::cout << "Command: \"" << this->_command << "\"" << std::endl;
	for (std::size_t i = 1; it != itend; i++)
	{
		std::cout << "Parameter n°" << i << ": \"" << *it << "\"" << std::endl;
		++it;
	}
	std::cout << "Text message: \"" << this->_text_message << "\"" << std::endl;
}
