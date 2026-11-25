#pragma once
#include "includes/Dispatcher.hpp"
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

class User;
class Channel;

class Server
{
	private:
		const uint16_t 				_port;				//port sur lequel ecouter (uint16_t == short == 65535, le nombre de port possible)
		std::string					_psswd;				//mot de passe
		int							_listenfd;			//socket(du serveur) qui attend les ecoutes
		bool						_running;			//status, permet de sortir de la boucle au bon moment
		std::vector<struct pollfd>	_pollfds;			// fd que la fonction poll() surveille
		std::map<int, User*>		_clients;			// retrouver un client via son fd (chaque client a un fd et un objet de la classe client)
		std::map<int, std::string>	_recvbuf;
		std::map<int, std::string>	_sendbuf;
		Dispatcher					*_dispatcher;		// A pointer the dispatcher object
		std::vector<Channel *>		_channel;			// A vector who will store every channel availables

	public:
		Server(const uint16_t port, const std::string &password);
		~Server();

		void Stop();
		void Run();
		void Accept();
		void SetupSocket();
		bool Receive(int fd);
		bool Send(size_t i);
		void QueueMessage(int fd, const std::string &msg);
		void Disconnect(size_t i);
};
