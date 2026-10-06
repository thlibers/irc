#pragma once

#include <exception>
#include <stdexcept>
#include <string>

class Server;

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
		void	help();
};
