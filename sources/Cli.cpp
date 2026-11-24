
#include "includes/Cli.hpp"
#include "includes/User.hpp"
#include "includes/Channel.hpp"
#include "includes/Dispatcher.hpp"
#include "includes/Message.hpp"
#include "includes/namespace/helper.hpp"
#include "includes/irc.hpp"
#include "includes/Server.hpp"

#include <inttypes.h>
#include <iostream>
#include <cstddef>
#include <exception>
#include <new>
#include <stdexcept>
#include <vector>
#include <map>

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
	if (this->_server != NULL)
		delete _server;
}

#ifdef DEBUG
static void	test_dispatcher(void)
{
	std::map<int, User *>	usr;
	std::vector<Channel *>	ch;
	Dispatcher				disp(&usr, &ch);
	char*					str = (char*)"PRIVMSG nick2 : Hello world!";

	usr[1] = new(std::nothrow) User("nick1", "usr1", "real", "pass");
	usr[2] = new(std::nothrow) User("nick2", "usr2", "real", "pass");
	usr[3] = new(std::nothrow) User("nick3", "usr3", "real", "pass");
	usr[4] = new(std::nothrow) User("nick4", "usr4", "real", "pass");
	usr[5] = new(std::nothrow) User("nick5", "usr5", "real", "pass");
	usr[7] = new(std::nothrow) User("nick6", "usr6", "real", "pass");

	for (std::map<int, User *>::iterator it = usr.begin(); it != usr.end(); ++it)
	{
		std::cout << "fd = " <<  (*it).first << " ; nickname = " << (*it).second->getNickname() << std::endl;
	}

	disp.executeCommand(5, str);
}
#endif /* DEBUG */
/*
 * @brief: Parse arguments from argv and run the server
 * @throw:
 *     - Cli::Cli_error a custom exception who print a personized what() message
 *       and the usage of the software
 * @todo:
 *     - Uncomment the Server creation and launch at the bottom of the function
 *       when the is ready to be launched (check destructor too).
 */
void	Cli::run(void)
{
	std::string password;
	uint16_t	port;

	if (DEBUG != false)
	{
		std::cerr << FR_BLUE
				  << "[?] The debug mode is activated"
				  << RESET << std::endl;
#ifdef DEBUG
		test_dispatcher();
#endif /* DEBUG */
	}
	if (this->_argc < 3)
		throw (Cli::Cli_error(ENOE_ARGS));
	else if (this->_argc > 3)
		throw (Cli::Cli_error(ETOM_ARGS));

	port = helper::checkPort(this->_argv[1]);
	password = helper::checkPassword(this->_argv[2]);

	std::cout << FR_GREEN << "[DEBUG] Server will listen on port: " << port <<
							 ", connect to it with the password \"" << password << "\""
			  << RESET << std::endl;

	try
	{
		this->_server = new Server(port, password);
		this->_server->Run();
	}
	catch (std::bad_alloc &ex)
	{
		std::cerr << FR_RED << "[!] Failed to allocated memory for the server!" << RESET << std::endl;
	}
}
/*
 * @brief: The function that display the help message
 */
void	Cli::help(void) throw()
{
	std::cerr << "Usage: " << _argv[0] << " {PORT} {PASSWORD}" << std::endl
			  << "    {PORT} = The number of the port to use" << std::endl
			  << "    {PASSWORD} = The password to connect to the server" << std::endl;
}
/*
 * =======================
 * = CLI_ERROR EXCEPTION =
 * =======================
 */
Cli::Cli_error::Cli_error(void) : std::runtime_error(""), _message("undefined cli_error") { ; }
Cli::Cli_error::Cli_error(const std::string message) : std::runtime_error(""), _message(message) { ; }
Cli::Cli_error::~Cli_error(void) throw() { ; }

const char *Cli::Cli_error::what(void) const throw()
{
	// Return the c version of the message give on the constructor of the exception
	return (_message.c_str());
}
