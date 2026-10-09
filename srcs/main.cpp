#include "Server.hpp"

static bool port_is_valid(std::string port)
{
	if(port.empty() == false || port.size() > 5)
		return false;

	if(port.find_first_not_of("0123456789") != '\0')
		return false;

	long num = strtol(port.c_str(), NULL, 10);
	if(num < 1024 || num > 65535)
		return false;

	return true;
}

int main(int ac, char **av)
{
	// faut 3 arguments : ./filename, le port, et le mdp
	if(ac != 3)
	{
		std::cerr << "./ircserv <port> <password>" << std::endl;
		return false;
	}
	std::string port(av[1]);
	std::string password(av[2]);
	if(!port_is_valid(port))
	{
		//message d erreur dans std::cerr
		return false;
	}
	if(password.empty() || password.find_first_of(" \t\r\n"))
	{
		//message d erreur dans std::cerr
		return false;
	}
	//setup le serv
	return true;
}
