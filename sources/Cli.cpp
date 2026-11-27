
#include "includes/Cli.hpp"
#include "includes/Logging.hpp"
#include "includes/User.hpp"
#include "includes/channel/Channel.hpp"
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

/*
 * @name   Cli
 * @brief  default constructor
 * @detail Set every field to 0/NULL
 */
Cli::Cli(void) : _server(NULL), _argc(0), _argv(NULL) { ; };
/*
 * @name   Cli
 * @brief  constructor to put set reference to the given parameter
 * @param  argc, the number of arguments that the user have entered
 * @param  argc, the array with every arguments types
 * @detail Set every field to 0/NULL except for _argc and _argv who are set to there
 *         corresponding value
 */
Cli::Cli(int &argc, char **&argv) : _server(NULL), _argc(argc), _argv(argv) { ; };
/*
 * @name   Cli
 * @brief  The destructor of the object
 * @detail Since the Server object is created during the execution of the CLI
 *         we destruct him if it exist
 */
Cli::~Cli(void)
{
	if (this->_server != NULL)
		delete _server;
}
/*
 * @name   run
 * @brief  Parse arguments from argv and run the server
 * @throw  Cli_error with the corresponding error write on it
 */
void	Cli::run(void)
{
	std::string password;
	uint16_t	port;

	/* Show who tell that DEBUG mode is activated if the user
	 * have compiled with -DDEBUG (e.g make re DEUBG=1) */
	#ifdef DEBUG
	Logging::warn() << "The debug mode is activated";
	#endif
	/* Check the arguments count (argc) */
	if (this->_argc < 3)
		throw (Cli::Cli_error(ENOE_ARGS));
	else if (this->_argc > 3)
		throw (Cli::Cli_error(ETOM_ARGS));

	/* Check that both given port and password by the user is valid */
	port = helper::checkPort(this->_argv[1]);
	password = helper::checkPassword(this->_argv[2]);

	/* Create and run the server without */
	this->_server = new(std::nothrow) Server(port, password);
	if (!this->_server)
		throw (std::runtime_error("Failed to allocated memory for the server!"));
	this->_server->Run();
}
/*
 * @name  help
 * @brief Display on the standard error output an help message
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

/*
 * @name Cli_error
 * @brief default constructor, set output message to a string that notify the user
 *        that the error is undefined
 */
Cli::Cli_error::Cli_error(void) : std::runtime_error(""), _message("undefined cli_error") { ; }
/*
 * @name  Cli_error
 * @brief Constructor who store the message write on the object
 */
Cli::Cli_error::Cli_error(const std::string message) : std::runtime_error(""), _message(message) { ; }
/*
 * @name  Cli_error
 * @brief Destructor
 */
Cli::Cli_error::~Cli_error(void) throw() { ; }
/*
 * @name  what
 * @brief Function overloading of the what function
 */
const char *Cli::Cli_error::what(void) const throw()
{
	// Return the c version of the message give on the constructor of the exception
	return (_message.c_str());
}
