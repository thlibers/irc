
#include "includes/Cli.hpp"
#include "includes/Logging.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

int main(int argc, char **argv)
{
	Cli cli(argc, argv);

	try
	{
		cli.run();
	}
	catch (Cli::Cli_error &ex)
	{
		Logging::error() << ex.what();
		cli.help();
		return (EXIT_FAILURE);
	}
	catch (std::exception &ex)
	{
		Logging::error() << ex.what();
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
