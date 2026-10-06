
#include "includes/Cli.hpp"
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
		std::cerr << "Error caught : " << ex.what() << std::endl;
		cli.help();
		return (1);
	}
	catch (std::exception &ex)
	{
		std::cerr << "Error caught : " << ex.what() << std::endl;
		return (1);
	}
	return (0);
}
