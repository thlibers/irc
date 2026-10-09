#include "includes/Server.hpp"
#include <unistd.h>

Server::Server(int port, const std::string &password): _port(port), _psswd(password), _listenfd(-1), _running(false) { }

// le but ici est de liberer chaque client -> fermer son fd, et delete sa "fiche" client
// pour se balader dans la map client on a besoin d un iterateur, end pointe sur l element apres le dernier (comme '\0' sur une string)
// first = le fd du client
// second = l objet client (nom, mdp, etc)
// si jamais le socket serveur ecoute, faut le close aussi (_listenfd)
Server::~Server()
{
	for (std::map<int, Client*>::iterator it = _clients.begin();
		it != _clients.end();
		++it)
	{
		close(it->first);
		// TODO: decommenter quand la classe Client existe (delete sur type incomplet = erreur avec -Werror)
		// delete it->second;
	}
	_clients.clear();
	if(_listenfd != -1)
		close(_listenfd);
}

void Server::Stop()
{
	_running = false;
}
void Server::Run()
{

}

void Server::SetupSocket()
{

}
