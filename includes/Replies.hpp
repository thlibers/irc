#pragma once

#include <string>
#include <sstream>
#include <iomanip>

#include "irc.hpp"

/*
 *	Constructeurs des reponses numeriques envoyees par le serveur.
 *
 *	Chaque fonction renvoie une ligne IRC complete, "\r\n" compris, prete a etre
 *	mise dans t_reply::line :
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

//	Modes annonces au client dans 004
#define USER_MODES		"o"
#define CHANNEL_MODES	"iklot"

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
}
