#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <inttypes.h>

#ifndef DEBUG
# define DEBUG false
#endif

class Server;

/*
 * @brief: Parse arguments from argv and launch the server
 * @detail: This class is the spine of our server. She is in charge of take
 *     information from our argv and check if the information provided is valid,
 *     non valid information can be such as short overflow, password too long or
 *     port who contain non digit character. The Server * is allocated dynamicly
 *     when we are sure that the provided information is valid and then the server
 *     is launch
 *
 */
class Cli
{
	private:
		Server	*_server;
		int		_argc;
		char	**_argv;

	public:
		Cli(void);
		Cli(int &argc, char **&argv);
		Cli(Cli &cpy);
		Cli &operator=(Cli &cpy);
		~Cli(void);

		class Cli_error : public std::runtime_error
		{
			private:
				std::string _message;
			public:
				Cli_error(void);
				Cli_error(const std::string message);
				virtual ~Cli_error(void) throw();
				virtual const char *what(void) const throw();
		};

		void	run();
		void	help() throw();
};
