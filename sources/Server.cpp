#include "includes/Server.hpp"
#include "includes/Dispatcher.hpp"
#include "includes/Logging.hpp"
#include "includes/User.hpp"
#include "includes/irc.hpp"
#include <new>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>

static volatile sig_atomic_t g_signal = 0;

/*
 * @brief: handler de SIGINT (Ctrl+C) et SIGQUIT (Ctrl+\). Leve g_signal pour
 *			que la boucle de Run() s arrete et que le serveur quitte proprement.
 */
static void handleSignal(int sig)
{
	(void)sig;
	g_signal = 1;
}

Server::Server(const uint16_t port, const std::string &password): _port(port), _psswd(password), _listenfd(-1), _running(false)
{
	// Create the dispatcher object, the "std::nothrow" avoid new to throw a std::bad_alloc exception.
	// Ive do this to get a nice message output instead of a regular "Allocation failed: std::bad_alloc"
	this->_dispatcher = new(std::nothrow) Dispatcher(this->_clients, this->_channel, this->_psswd);
	if (!this->_dispatcher)
		throw (std::runtime_error("Failed to allocated memory for the command dispatcher"));
}

// le but ici est de liberer chaque client -> fermer son fd, et delete sa "fiche" client
// pour se balader dans la map client on a besoin d un iterateur, end pointe sur l element apres le dernier (comme '\0' sur une string)
// first = le fd du client
// second = l objet client (nom, mdp, etc)
// si jamais le socket serveur ecoute, faut le close aussi (_listenfd)
Server::~Server()
{
	if (this->_dispatcher)
		delete (this->_dispatcher);
	for (std::map<int, User*>::iterator it = _clients.begin();
		it != _clients.end();
		++it)
	{
		close(it->first);
		delete it->second;
	}
	_clients.clear();
	if(_listenfd != -1)
		close(_listenfd);
}

void Server::Stop()
{
	_running = false;
}

/*
* @brief:	boucle principale du serveur. Attend des evenements sur l ensemble
*			des fd surveilles (socket d ecoute + clients) et les traite, jusqu a
*			ce que _running passe a false.
* @throw:
*		- std::runtime_error si SetupSocket() echoue, ou si poll() echoue pour
*		  une autre raison qu une interruption par signal
* @note:
*		- c est le seul endroit qui appelle SetupSocket()
*/
void Server::Run()
{
	//premiere ligne handle le Ctrl + C
	// deuxieme ligne handle Ctrl + \ ;
	// troisieme ligne evite que le serv crash auto si on veut
	// ecrire sur une connexion fermee que le serv a pas encore traitee (ca ignore ce signal SIGPIPE)
	signal(SIGINT, handleSignal);
	signal(SIGQUIT, handleSignal);
	signal(SIGPIPE, SIG_IGN);

	SetupSocket();
	_running = true;
	Logging::info() << "The server has been launched";
	while (_running && !g_signal)
	{
		// poll() ne lit ni n ecrit rien : ca endort le processus jusqu a ce qu au
		// moins un fd surveille soit pret, et ca renseigne les champs revents et
		// renvoie le nombre de fd concernes. ca evite d avoir 100% d utilisation cpu
		// car ca attend et ca interroge pas en boucle.
		// et ca evite aussi  de bloquer sur un seul client et donc de geler le serv
		int ready = poll(&_pollfds[0], _pollfds.size(), -1);

		if (ready == -1)
		{
			if (errno == EINTR)   // un signal a interrompu l attente, pas une erreur
				continue;         // on repart au while, qui retestera _running
			throw std::runtime_error("poll() failed");
		}

		// ici on sait que ready > 0 : au moins un fd a un evenement.
		for(size_t i = 0; i < _pollfds.size(); i++)
		{
			if(_pollfds[i].revents == 0)
				continue;
			if(_pollfds[i].fd == _listenfd)
			{
				if(_pollfds[i].revents & POLLIN)
				{
					Accept();
				}
			}
			else
			{
				// alive passe a false des qu une etape dit "ce client est a deconnecter" :
				// ca evite d appeler Send() sur un client mort et de deconnecter deux fois
				bool alive = true;

				// POLLIN = donnees a lire. POLLHUP / POLLERR = connexion coupee ou en erreur :
				// on appelle quand meme Receive(), recv() renverra 0 ou -1 et donc false
				if (_pollfds[i].revents & (POLLIN | POLLHUP | POLLERR))
					alive = Receive(_pollfds[i].fd);
				// POLLOUT = le client peut recevoir, on vide son buffer d envoi
				if (alive && (_pollfds[i].revents & POLLOUT))
					alive = Send(i);
				if (!alive)
				{
					Disconnect(i);
					// la case i a ete supprimee, la suivante a pris sa place :
					// on recule pour ne pas la sauter au i++ de la boucle
					i--;
				}
			}
		}
	}
	Logging::info() << "Server have been stopped";
}

/*
 * @brief: prepare le socket d ecoute du serveur. Apres l appel, le serveur est
 *         joignable sur _port : le noyau accepte les connexions entrantes et les
 *         empile dans une file, que Run() recuperera avec accept().
 * @throw:
 *     - std::runtime_error si un des appels echoue (socket, setsockopt, fcntl,
 *       bind, listen)
 * @note:
 *     - modifie _listenfd et ajoute la premiere entree de _pollfds
 *     - _listenfd ne transporte jamais de donnees : il ne sert qu a reserver le
 *       port et a produire de nouveaux fd via accept()
 *     - doit etre appelee une seule fois, au debut de Run()
 */
void Server::SetupSocket()
{
	// SOCK_STREAM = TCP : flux fiable et ordonne, indispensable pour IRC
	_listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenfd == -1)
		throw std::runtime_error("socket() failed");

	// permet de relancer le serveur sans attendre la fin du TIME_WAIT de TCP,
	// sinon bind() echoue avec EADDRINUSE. A poser avant bind(), qui lit l option.
	int opt = 1;
	if (setsockopt(_listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt() failed");

	// serveur mono-thread : un seul appel bloquant gelerait tous les clients
	if (fcntl(_listenfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl() failed");

	// memset a cause du champ de bourrage sin_zero, qui doit valoir 0.
	// htons / htonl : les en-tetes reseau sont en big endian, x86 en little endian.
	// INADDR_ANY (0.0.0.0) = ecouter sur toutes les interfaces de la machine.
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind() reserve le port pour ce processus, mais les connexions sont encore
	// refusees. Cast impose par l API, generique (IPv4, IPv6, sockets unix).).
	if (bind(_listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		throw std::runtime_error("bind() failed");

	// listen()  bascule le socket en mode passif :
	// le noyau prend en charge les poignees de main TCP seul et empile les connexions etablies.
	if (listen(_listenfd, SOMAXCONN) == -1)
		throw std::runtime_error("listen() failed");

	// events = ce qu on demande au noyau, revents = ce qu il y a trouve (il l ecrit).
	// POLLIN sur un socket d ecoute = "une connexion attend", pas "des donnees a lire".
	struct pollfd pfd;
	pfd.fd = _listenfd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
}

/*
 * @brief: recupere une connexion en attente sur _listenfd et commence a
 *         surveiller le nouveau client.
 * @note:
 *     - le fd client est passe en non-bloquant (ca ne s herite pas de _listenfd)
 *     - un echec n est pas fatal : on abandonne cette connexion, sans throw
 */
void Server::Accept()
{
	int fd = accept(_listenfd, NULL, NULL);
	if (fd == -1)
		return;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(fd);
		return;
	}
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfds.push_back(pfd);

	// Create the "User" object associated with the fd
	_clients[fd] = new(std::nothrow) User();
	if (!this->_clients[fd])
	{
		Logging::error() << "Failed to create the client for fd " << fd;

		// pas de send() ici : le sujet interdit tout send()/recv() hors de poll().
		// On abandonne simplement la connexion, le client verra le socket se fermer
		close(fd);
		_pollfds.pop_back();
		_clients.erase(fd);
	}
	else
	{
		this->_clients[fd]->setFd(fd);
		Logging::ok() << "New Client FD : " << fd;
	}
}

/*
 * @brief: lit ce que le client a envoye (un seul recv() par reveil de poll())
 *         et traite chaque ligne complete de son buffer de reception.
 * @param: fd, le socket du client
 * @return: false si le client est a deconnecter (connexion fermee ou erreur)
 * @note:
 *     - une ligne incomplete reste dans _recvbuf jusqu au prochain recv()
 *     - pour l instant chaque ligne est renvoyee au client (echo de test)
 */
bool Server::Receive(int fd)
{
	char buf[512];
	ssize_t n = recv(fd, buf, sizeof(buf), 0);
	if(n <= 0)
		return false;
	_recvbuf[fd].append(buf, n);
	size_t pos = _recvbuf[fd].find("\n");
	while (pos != std::string::npos)
	{
		// la ligne = tout ce qui precede le \n
		std::string line = _recvbuf[fd].substr(0, pos);
		// on retire du buffer la ligne et son \n (d'ou le + 1)
		_recvbuf[fd].erase(0, pos + 1);
		// IRC termine les lignes par \r\n : on enleve le \r s il est la.
		// le test !empty() evite de lire line[-1] sur une ligne vide
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		QueueMessage(fd, line + "\r\n");

		// le buffer a change : on recherche la ligne suivante
		pos = _recvbuf[fd].find("\n");
	}
	return true;
}

/*
 * @brief: ferme la connexion d un client et oublie tout ce qui le concerne.
 * @param: i, l index du client dans _pollfds (pas son fd)
 * @note:
 *     - supprime la case i : l appelant doit reculer son index de boucle
 */
void Server::Disconnect(size_t i)
{
	int fd = _pollfds[i].fd;
	close(fd);
	// find() et pas _clients[fd] : operator[] recreerait une entree si elle n existe pas.
	// On delete l objet AVANT d effacer l entree, sinon on perd le pointeur (fuite)
	std::map<int, User*>::iterator it = _clients.find(fd);
	if (it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}
	_pollfds.erase(_pollfds.begin() + i);
	_recvbuf.erase(fd);
	_sendbuf.erase(fd);
	Logging::ok() << "Client : " << fd << " disconnected";
}

/*
 * @brief: met un message en attente d envoi pour un client. Rien n est envoye
 *         ici : on active POLLOUT et c est Send() qui fera le send().
 * @param: fd, le socket du destinataire
 * @param: msg, le texte a envoyer, "\r\n" compris
 */
void Server::QueueMessage(int fd, const std::string &msg)
{
	uint16_t return_code;

	(void)return_code;
	if (msg.empty())
		return;
	/* Dispatcher problaly here */
	return_code = _dispatcher->executeCommand(fd, msg);

	/* Placeholder that return the message sent by the user */
	_sendbuf[fd] += msg;
	for(size_t i = 0; i < _pollfds.size(); i++)
	{
		if(_pollfds[i].fd == fd)
		{
			_pollfds[i].events |= POLLOUT;
		}
	}
}

/*
 * @brief: envoie au client ce que son buffer d envoi contient. Si tout n est
 *         pas parti, le reste attend le prochain POLLOUT.
 * @param: i, l index du client dans _pollfds
 * @return: false si le client est a deconnecter
 * @note:
 *     - POLLOUT est desactive une fois le buffer vide, sinon poll() se
 *       reveillerait en boucle
 */
bool Server::Send(size_t i)
{
	int fd = _pollfds[i].fd;
	ssize_t n = send(fd, _sendbuf[fd].c_str(), _sendbuf[fd].size(), MSG_NOSIGNAL);
	if(n <= 0)
		return false;
	_sendbuf[fd].erase(0, n);
	if(_sendbuf[fd].empty())
		_pollfds[i].events &= ~POLLOUT;
	return true;
}
