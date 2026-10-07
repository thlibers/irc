#include "includes/Cli.hpp"
#include "includes/Message.hpp"

#include <inttypes.h>
#include <iostream>
#include <cstddef>
#include <exception>

/*
 * =============
 * = CLI CLASS =
 * =============
 */

Cli::Cli(void) : _server(NULL), _argc(0), _argv(NULL) { ; };

Cli::Cli(int &argc, char **&argv) : _server(NULL), _argc(argc), _argv(argv) { ; };

Cli::Cli(Cli &cpy) : _server(NULL), _argc(cpy._argc), _argv(cpy._argv) { ; };

Cli	&Cli::operator=(Cli &cpy)
{
	if (&cpy != this)
	{
		this->_server = cpy._server;
		this->_argc = cpy._argc;
		this->_argv = cpy._argv;
	}
	return (*this);
}

Cli::~Cli(void)
{
	// if (this->_server != NULL)
		// delete _server;
	;
}

/*
 * @brief: Parse arguments from argv and run the server
 */
void	Cli::run(void)
{
	std::string password;
	uint16_t	port;

	(void)port;
	if (this->_argc < 3)
		throw (Cli::Cli_error("Not enought arguments!"));
	else if (this->_argc > 3)
			throw (Cli::Cli_error("Too enought arguments!"));

	// Server launch goes here,
	// this->_server = new Server(port, password)
	try
	{
		char	*raw = (char *)"PRIVMSG #channel : Hello world\r\n";
		Message	msg(raw);
		msg.parse();
		std::cout << "Command: " << msg.getCommand() << std::endl
				  << "First parameter: " <<  msg.getParameter()[0] << std::endl
				  << "Text message: \"" << msg.getTextMessage() << "\"" << std::endl;
	}
	catch (std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	std::cout << "NORMAL USE" << std::endl;
}
/*
 * @brief: The function that display the help message
 */
void	Cli::help(void)
{
	std::cerr << "Usage: " << _argv[0] << " {IP} {PASSWORD}" << std::endl;
}

/*
 * =======================
 * = CLI_ERROR EXCEPTION =
 * =======================
 */

Cli::Cli_error::Cli_error(void) : std::runtime_error(""), _message("") { ; }
Cli::Cli_error::Cli_error(const std::string message) : std::runtime_error(""), _message(message) { ; }
Cli::Cli_error::~Cli_error(void) throw() { ; }

const char *Cli::Cli_error::what(void) const throw()
{
	return (_message.c_str());
}
