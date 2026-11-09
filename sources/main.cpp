
#include "includes/Cli.hpp"
#include "includes/helper.hpp"
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
		std::cerr << FR_RED << "[!] Error : " << ex.what() << RESET << std::endl;
		cli.help();
		return (1);
	}
	catch (std::exception &ex)
	{
		std::cerr << FR_RED << "[!] Error : " << ex.what() << RESET << std::endl;
		return (1);
	}
	return (0);
}
