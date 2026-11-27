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
		std::string					_create_at;			// Date et heure de lancement du serveur, envoyee dans 003 RPL_CREATED

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

		// --- GETTER ---
		// Used by the Dispatcher to reach the users, the channels and the
		// password of the server through the Server class
		std::map<int, User *>		&getClients(void);
		std::vector<Channel *>		&getChannels(void);
		std::string					&getPassword(void);
		const std::string			&getCreateAt(void) const;
};
