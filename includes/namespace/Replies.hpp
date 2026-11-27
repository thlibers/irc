#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <inttypes.h>

#include "includes/Logging.hpp"
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/irc.hpp"

/*
 *	Constructeurs des reponses numeriques envoyees par le serveur.
 *
 *	Chaque fonction renvoie une ligne IRC complete, "\r\n" compris, prete a etre
 *	mise dans le buffer d'envoi du destinataire :
 *
 *		:<serveur> <code> <client> <parametres...> [:<texte>]\r\n
 *
 *	- <serveur> : SERVER_NAME, indique que c'est le serveur qui parle
 *	- <code>    : le numerique sur 3 chiffres (cf. irc.hpp pour la liste complete)
 *	- <client>  : le nick du destinataire, "*" s'il n'a pas encore de nick
 *	              (passer par rpl::client() quand ce cas est possible)
 *
 *	Formats : https://modern.ircdocs.horse/#numerics (ce que suivent irssi, weechat, hexchat).
 *
 *	Pourquoi des fonctions et pas des macros : irc.hpp definit deja RPL_WELCOME, ERR_... comme
 *	des nombres, une macro-fonction du meme nom serait une redefinition (erreur avec -Werror).
 *	Elles sont "inline" pour pouvoir etre definies dans le header sans "multiple definition"
 *	au link quand plusieurs .cpp l'incluent.
 */

#define SERVER_NAME		"ircserv"
#define SERVER_VERSION	"ircserv-1.0"
#define USERLEN			12 // The number of character that a username can have
#define REALLEN			128 // The number of character possible in a realname

//	Modes annonces au client dans 004
#define USER_MODES		"o"
#define CHANNEL_MODES	"iklot"

/*
 * @namespace rpl
 * @note <doxygene to write>
 */
namespace rpl
{
	//	Le nick a mettre dans le champ <client> : "*" tant que NICK n'a pas ete recu
	inline std::string	client(const std::string &nick)
	{
		return (nick.empty() ? "*" : nick);
	}

	//	Convertit un code de irc.hpp (RPL_/ERR_, definis comme des nombres) en
	//	numerique IRC sur 3 chiffres, ex. RPL_YOURHOST -> "002"
	inline std::string	code(int numeric)
	{
		std::ostringstream	stream;

		stream << std::setw(3) << std::setfill('0') << numeric;
		return (stream.str());
	}

	//	Assemble ":<serveur> <code> <client> <params>\r\n"
	//	params contient deja le ':' devant le dernier parametre quand il en faut un
	inline std::string	build(const std::string &code, const std::string &client, const std::string &params)
	{
		return (":" SERVER_NAME " " + code + " " + client + " " + params + "\r\n");
	}

	//	Enregistrement : envoyes une fois quand PASS + NICK + USER sont valides.
	//	Le client se considere connecte a la reception de 001.

	//	001 RPL_WELCOME
	inline std::string	welcome(const std::string &nick, const std::string &user, const std::string &host)
	{
		return (build(code(RPL_WELCOME), nick, ":Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host));
	}

	//	002 RPL_YOURHOST
	inline std::string	yourHost(const std::string &nick)
	{
		return (build(code(RPL_YOURHOST), nick, ":Your host is " SERVER_NAME ", running version " SERVER_VERSION));
	}

	//	003 RPL_CREATED, date = date de lancement du serveur
	inline std::string	created(const std::string &nick, const std::string &date)
	{
		return (build(code(RPL_CREATED), nick, ":This server was created " + date));
	}

	//	004 RPL_MYINFO : <serveur> <version> <modes user> <modes channel>
	inline std::string	myInfo(const std::string &nick)
	{
		return (build(code(RPL_MYINFO), nick, SERVER_NAME " " SERVER_VERSION " " USER_MODES " " CHANNEL_MODES));
	}

	//	005 RPL_ISUPPORT : decrit le fonctionnement du serveur au client
	//	CHANTYPES=#&       prefixes de channel acceptes
	//	PREFIX=(o)@        seul le statut operateur (@) existe
	//	CHANMODES=,k,l,it  listes (aucune), param toujours (k), param a l'ajout (l), sans param (it)
	inline std::string	iSupport(const std::string &nick)
	{
		return (build(code(RPL_ISUPPORT), nick, "CHANTYPES=#& PREFIX=(o)@ CHANMODES=,k,l,it :are supported by this server"));
	}

	//	221 RPL_UMODEIS : reponse a "MODE <son_nick>"
	inline std::string	uModeIs(const std::string &nick, const std::string &modes)
	{
		return (build(code(RPL_UMODEIS), nick, modes));
	}

	//	324 RPL_CHANNELMODEIS : reponse a "MODE #chan", modes = "+itkl", args = " cle 10" ou ""
	inline std::string	channelModeIs(const std::string &nick, const std::string &channel,
										const std::string &modes, const std::string &args)
	{
		return (build(code(RPL_CHANNELMODEIS), nick, channel + " " + modes + args));
	}

	//	331 RPL_NOTOPIC : reponse a "TOPIC #chan" quand il n'y a pas de topic
	inline std::string	noTopic(const std::string &nick, const std::string &channel)
	{
		return (build(code(RPL_NOTOPIC), nick, channel + " :No topic is set"));
	}

	//	332 RPL_TOPIC : au JOIN (si topic) et en reponse a "TOPIC #chan"
	inline std::string	topic(const std::string &nick, const std::string &channel, const std::string &topic)
	{
		return (build(code(RPL_TOPIC), nick, channel + " :" + topic));
	}

	//	341 RPL_INVITING : confirme a celui qui invite que l'invitation est partie
	inline std::string	inviting(const std::string &nick, const std::string &target, const std::string &channel)
	{
		return (build(code(RPL_INVITING), nick, target + " " + channel));
	}

	//	353 RPL_NAMREPLY, names = "@alice bob" (@ devant les operateurs)
	//	"=" : channel public ("@" secret et "*" prive n'existent pas ici)
	inline std::string	namReply(const std::string &nick, const std::string &channel, const std::string &names)
	{
		return (build(code(RPL_NAMREPLY), nick, "= " + channel + " :" + names));
	}

	//	366 RPL_ENDOFNAMES : toujours apres le(s) 353, meme si le channel n'existe pas
	inline std::string	endOfNames(const std::string &nick, const std::string &channel)
	{
		return (build(code(RPL_ENDOFNAMES), nick, channel + " :End of /NAMES list"));
	}

	//	368 RPL_ENDOFBANLIST : les clients demandent "MODE #chan b" au JOIN, pas de bans ici
	inline std::string	endOfBanList(const std::string &nick, const std::string &channel)
	{
		return (build(code(RPL_ENDOFBANLIST), nick, channel + " :End of channel ban list"));
	}
}
/*
 * @namespace err
 * @note <doxygene to write>
 */
namespace err
{
	using rpl::build;
	using rpl::code;

	//	401 ERR_NOSUCHNICK : PRIVMSG / INVITE / KICK vers un nick ou channel inexistant
	inline std::string	noSuchNick(const std::string &nick, const std::string &target)
	{
		return (build(code(ERR_NOSUCHNICK), nick, target + " :No such nick/channel"));
	}

	//	403 ERR_NOSUCHCHANNEL
	inline std::string	noSuchChannel(const std::string &nick, const std::string &channel)
	{
		return (build(code(ERR_NOSUCHCHANNEL), nick, channel + " :No such channel"));
	}

	//	404 ERR_CANNOTSENDTOCHAN : PRIVMSG vers un channel dont on n'est pas membre
	inline std::string	cannotSendToChan(const std::string &nick, const std::string &channel)
	{
		return (build(code(ERR_CANNOTSENDTOCHAN), nick, channel + " :Cannot send to channel"));
	}

	//	409 ERR_NOORIGIN : PING sans parametre
	inline std::string	noOrigin(const std::string &nick)
	{
		return (build(code(ERR_NOORIGIN), nick, ":No origin specified"));
	}

	//	402 ERR_NOSUCHSERVER : PING vers un serveur inconnu
	inline std::string	noSuchServer(const std::string &nick, const std::string &server)
	{
		return (build(code(ERR_NOSUCHSERVER), nick, server + " :No such server"));
	}

	//	411 ERR_NORECIPIENT : PRIVMSG / NOTICE sans cible
	inline std::string	noRecipient(const std::string &nick, const std::string &command)
	{
		return (build(code(ERR_NORECIPIENT), nick, ":No recipient given (" + command + ")"));
	}

	//	412 ERR_NOTEXTTOSEND : PRIVMSG sans texte
	inline std::string	noTextToSend(const std::string &nick)
	{
		return (build(code(ERR_NOTEXTTOSEND), nick, ":No text to send"));
	}

	//	421 ERR_UNKNOWNCOMMAND
	inline std::string	unknownCommand(const std::string &nick, const std::string &command)
	{
		return (build(code(ERR_UNKNOWNCOMMAND), nick, command + " :Unknown command"));
	}

	//	422 ERR_NOMOTD : peut remplacer le MOTD apres 001-005
	inline std::string	noMotd(const std::string &nick)
	{
		return (build(code(ERR_NOMOTD), nick, ":MOTD File is missing"));
	}

	//	431 ERR_NONICKNAMEGIVEN
	inline std::string	noNicknameGiven(const std::string &nick)
	{
		return (build(code(ERR_NONICKNAMEGIVEN), nick, ":No nickname given"));
	}

	//	432 ERR_ERRONEUSNICKNAME : caractere interdit, commence par un chiffre...
	inline std::string	erroneusNickname(const std::string &nick, const std::string &badNick)
	{
		return (build(code(ERR_ERRONEUSNICKNAME), nick, badNick + " :Erroneus nickname"));
	}

	//	433 ERR_NICKNAMEINUSE
	inline std::string	nicknameInUse(const std::string &nick, const std::string &badNick)
	{
		return (build(code(ERR_NICKNAMEINUSE), nick, badNick + " :Nickname is already in use"));
	}

	//	441 ERR_USERNOTINCHANNEL : KICK / MODE +o sur quelqu'un qui n'est pas dans le channel
	inline std::string	userNotInChannel(const std::string &nick, const std::string &target, const std::string &channel)
	{
		return (build(code(ERR_USERNOTINCHANNEL), nick, target + " " + channel + " :They aren't on that channel"));
	}

	//	442 ERR_NOTONCHANNEL : l'emetteur lui-meme n'est pas dans le channel
	inline std::string	notOnChannel(const std::string &nick, const std::string &channel)
	{
		return (build(code(ERR_NOTONCHANNEL), nick, channel + " :You're not on that channel"));
	}

	//	443 ERR_USERONCHANNEL : INVITE de quelqu'un deja present
	inline std::string	userOnChannel(const std::string &nick, const std::string &target, const std::string &channel)
	{
		return (build(code(ERR_USERONCHANNEL), nick, target + " " + channel + " :is already on channel"));
	}

	//	451 ERR_NOTREGISTERED : commande autre que PASS/NICK/USER/CAP/QUIT avant 001
	inline std::string	notRegistered(const std::string &nick)
	{
		return (build(code(ERR_NOTREGISTERED), nick, ":You have not registered"));
	}

	//	461 ERR_NEEDMOREPARAMS
	inline std::string	needMoreParams(const std::string &nick, const std::string &command)
	{
		return (build(code(ERR_NEEDMOREPARAMS), nick, command + " :Not enough parameters"));
	}

	//	462 ERR_ALREADYREGISTRED : PASS ou USER apres l'enregistrement
	inline std::string	alreadyRegistered(const std::string &nick)
	{
		return (build(code(ERR_ALREADYREGISTRED), nick, ":You may not reregister"));
	}

	//	464 ERR_PASSWDMISMATCH
	inline std::string	passwdMismatch(const std::string &nick)
	{
		return (build(code(ERR_PASSWDMISMATCH), nick, ":Password incorrect"));
	}

	//	468 ERR_INVALIDUSERNAME : USER avec un caractere interdit dans le username
	inline std::string	invalidUsername(const std::string &nick)
	{
		return (build(code(ERR_INVALIDUSERNAME), nick, ":Invalid username"));
	}

	//	471 ERR_CHANNELISFULL : limite +l atteinte
	inline std::string	channelIsFull(const std::string &nick, const std::string &channel)
	{
		return (build(code(ERR_CHANNELISFULL), nick, channel + " :Cannot join channel (+l)"));
	}

	//	472 ERR_UNKNOWNMODE : lettre de mode non geree
	inline std::string	unknownMode(const std::string &nick, char mode)
	{
		return (build(code(ERR_UNKNOWNMODE), nick, std::string(1, mode) + " :is unknown mode char to me"));
	}

	//	473 ERR_INVITEONLYCHAN : +i et pas invite
	inline std::string	inviteOnlyChan(const std::string &nick, const std::string &channel)
	{
		return (build(code(ERR_INVITEONLYCHAN), nick, channel + " :Cannot join channel (+i)"));
	}

	//	475 ERR_BADCHANNELKEY : +k et mauvaise cle
	inline std::string	badChannelKey(const std::string &nick, const std::string &channel)
	{
		return (build(code(ERR_BADCHANNELKEY), nick, channel + " :Cannot join channel (+k)"));
	}

	//	476 ERR_BADCHANMASK : nom de channel mal forme
	inline std::string	badChanMask(const std::string &nick, const std::string &channel)
	{
		return (build(code(ERR_BADCHANMASK), nick, channel + " :Bad Channel Mask"));
	}

	//	482 ERR_CHANOPRIVSNEEDED : KICK / INVITE(+i) / TOPIC(+t) / MODE sans etre operateur
	inline std::string	chanOPrivsNeeded(const std::string &nick, const std::string &channel)
	{
		return (build(code(ERR_CHANOPRIVSNEEDED), nick, channel + " :You're not channel operator"));
	}

	//	501 ERR_UMODEUNKNOWNFLAG
	inline std::string	uModeUnknownFlag(const std::string &nick)
	{
		return (build(code(ERR_UMODEUNKNOWNFLAG), nick, ":Unknown MODE flag"));
	}

	//	502 ERR_USERSDONTMATCH : "MODE autre_nick ..."
	inline std::string	usersDontMatch(const std::string &nick)
	{
		return (build(code(ERR_USERSDONTMATCH), nick, ":Cant change mode for other users"));
	}

	//	696 ERR_INVALIDMODEPARAM : ex. "MODE #c +l abc" -> mode = 'l', param = "abc"
	inline std::string	invalidModeParam(const std::string &nick, const std::string &target,
										char mode, const std::string &param,
										const std::string &description)
	{
		return (build(code(ERR_INVALIDMODEPARAM), nick, target + " " + std::string(1, mode) + " " + param + " :" + description));
	}
	//	PONG : echo du token d un PING (keep-alive), pas un numerique : la
	//	ligne se construit a la main, elle ne passe pas par build()
	inline std::string	pong(const std::string &token)
	{
		return (":" SERVER_NAME " PONG " + token + "\r\n");
	}
}
/*
 * @namespace replies
 * @brief The namespace with everything to build properly formed replies
 * @detail With this namespace, we are now able to convert a code (like
 *         001 welcome, 401 nosuchnick, ...) into the real error message to
 *         sent to the user.
 * @see rpl, err
 * @note
 *       - Every parameter is regoup in the t_return_info struct to centrilized everything
 *       at the same place.
 *       - Is used by the dispatcher
 */
namespace replies
{
	// Import other namespace on the replies namespace
	using namespace err;
	using namespace rpl;

	typedef enum s_code_type
	{
		CODE_TYPE_UNKNOWN = 0,
		CODE_TYPE_ERROR,
		CODE_TYPE_REPLY,
	}				t_code_type;
	// struct who contain every information to build the message
	typedef struct s_return_info
	{
		std::string	nick;					//	The nickname of the user how will recieve the message
		std::string	user;					//	The username of the usr who will recieve the message
		std::string	host;					//	The hostname is just the server name
		std::string	date;					//	When the server was started, needed by 003
		std::string	command;				//	The command who have trigger the reply
		std::string	target;					//	The faulty nick / channel, needed by 401 / 432 / 433 / ...
		uint16_t	exit_code;				// The code who is returned by the handler, 0 = nothing to send
		uint8_t		snapshot_registration;	// Snapshot of the 8 bits
		int			fd;

		s_return_info(void) :
			nick("*"),
			user(),
			host(SERVER_NAME),
			date(),
			command(),
			target(),
			exit_code(0),
			snapshot_registration(0),
			fd(-1)
		{ ; }
		s_return_info(const ::Message &msg) :
			nick(),
			user(),
			host(SERVER_NAME),
			date(),
			command(msg.getCommand()),
			target(),
			exit_code(0),
			snapshot_registration(0),
			fd(-1)
		{ ; }
		s_return_info(const ::User &user, const ::Message &msg) :
			nick(rpl::client(user.getNickname())),
			user(user.getUsername()),
			host(SERVER_NAME),
			date(),
			command(msg.getCommand()),
			target(),
			exit_code(0),
			snapshot_registration(user.isRegisted()),
			fd(user.getFd())
		{ ; }
		s_return_info(const ::User &user, const ::Message &msg, int code) :
			nick(rpl::client(user.getNickname())),
			user(user.getUsername()),
			host(SERVER_NAME),
			date(),
			command(msg.getCommand()),
			target(),
			exit_code(code),
			snapshot_registration(user.isRegisted()),
			fd(user.getFd())
		{ ; }
		s_return_info(const ::User &user, const ::Message &msg, const std::string &date) :
			nick(rpl::client(user.getNickname())),
			user(user.getUsername()),
			host(SERVER_NAME),
			date(date),
			command(msg.getCommand()),
			target(),
			exit_code(0),
			snapshot_registration(user.isRegisted()),
			fd(user.getFd())
		{ ; }
		void clear(void)
		{nick.clear(), user.clear(), host.clear(), date.clear(), command.clear(), target.clear(), exit_code = 0, fd = -1;}
	}	t_return_info;
	typedef t_return_info rtn_i_t;

	inline std::string	build(uint16_t code, const rtn_i_t &rtn_i)
	{
		switch (code)
		{
			//	Registration burst, sent once the three flags are up
			case RPL_WELCOME:			return (rpl::welcome(rtn_i.nick, rtn_i.user, rtn_i.host));
			case RPL_YOURHOST:			return (rpl::yourHost(rtn_i.nick));
			case RPL_CREATED:			return (rpl::created(rtn_i.nick, rtn_i.date));
			case RPL_MYINFO:			return (rpl::myInfo(rtn_i.nick));
			case RPL_ISUPPORT:			return (rpl::iSupport(rtn_i.nick));
			case ERR_NOMOTD:			return (err::noMotd(rtn_i.nick));

			//	Connection / registration errors
			case ERR_NEEDMOREPARAMS:	return (err::needMoreParams(rtn_i.nick, rtn_i.command));
			case ERR_ALREADYREGISTRED:	return (err::alreadyRegistered(rtn_i.nick));
			case ERR_PASSWDMISMATCH:	return (err::passwdMismatch(rtn_i.nick));
			case ERR_NOTREGISTERED:		return (err::notRegistered(rtn_i.nick));
			case ERR_NONICKNAMEGIVEN:	return (err::noNicknameGiven(rtn_i.nick));
			case ERR_ERRONEUSNICKNAME:	return (err::erroneusNickname(rtn_i.nick, rtn_i.target));
			case ERR_NICKNAMEINUSE:		return (err::nicknameInUse(rtn_i.nick, rtn_i.target));
			case ERR_INVALIDUSERNAME:	return (err::invalidUsername(rtn_i.nick));

			//	PRIVMSG / command errors
			case ERR_NORECIPIENT:		return (err::noRecipient(rtn_i.nick, rtn_i.command));
			case ERR_NOTEXTTOSEND:		return (err::noTextToSend(rtn_i.nick));
			case ERR_NOSUCHNICK:		return (err::noSuchNick(rtn_i.nick, rtn_i.target));
			case ERR_CANNOTSENDTOCHAN:	return (err::cannotSendToChan(rtn_i.nick, rtn_i.target));
			case ERR_BADCHANMASK:		return (err::badChanMask(rtn_i.nick, rtn_i.target));
			case ERR_UNKNOWNCOMMAND:	return (err::unknownCommand(rtn_i.nick, rtn_i.command));

			//	PING errors
			case ERR_NOORIGIN:		return (err::noOrigin(rtn_i.nick));
			case ERR_NOSUCHSERVER:	return (err::noSuchServer(rtn_i.nick, rtn_i.target));

			default:					return ("");
		}
	}
}
