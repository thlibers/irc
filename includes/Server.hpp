#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <map>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <inttypes.h> // uint16_t lib

class Client;

class Server
{
	private:
		const uint16_t 				_port;				//port sur lequel ecouter (uint16_t == short == 65535, le nombre de port possible)
		std::string					_psswd;				//mot de passe
		int							_listenfd;			//socket(du serveur) qui attend les ecoutes
		bool						_running;			//status, permet de sortir de la boucle au bon moment
		std::vector<struct pollfd>	_pollfds;			// fd que la fonction poll() surveille
		std::map<int, Client*>		_clients;			// retrouver un client via son fd (chaque client a un fd et un objet de la classe client)

	public:
		Server(const uint16_t port, const std::string &password);
		~Server();

		void Stop();
		void Run();
		void SetupSocket();
};
