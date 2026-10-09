#include "includes/Server.hpp"
#include <unistd.h>
#include <cerrno>

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
	SetupSocket();
	_running = true;
	while (_running)
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
		// C est ici que viendra le parcours de _pollfds (prochaine etape).
		std::cout << ready << " fd(s) pret(s)" << std::endl;
	}
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

