#include "includes/channel/Commands.hpp"
#include "includes/User.hpp"
#include "includes/Replies.hpp"
#include <cctype>
#include <sstream>

//	Helpers

static std::string toLower(std::string str)
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = std::tolower(static_cast <unsigned char>(str[i]));
	return (str);
}

//	Garde les morceaux vides pour que les cles de "JOIN #a,#b ,cle" restent alignees sur les channels
static std::vector <std::string> split(const std::string &str, char sep)
{
	std::vector <std::string> parts;
	std::string part;
	std::istringstream stream(str);

	while (std::getline(stream, part, sep))
		parts.push_back(part);
	return (parts);
}

static bool isChannelName(const std::string &name)
{
	return (!name.empty() && (name[0] == '#' || name[0] == '&'));
}

//	Un prefixe + au moins un caractere, 50 maximum, sans espace, virgule, ":" ni caractere de controle
static bool isValidChannelName(const std::string &name)
{
	if (!isChannelName(name) || name.size() < 2 || name.size() > 50)
		return (false);
	for (size_t i = 1; i < name.size(); ++i)
	{
		unsigned char c = name[i];
		if (c == ' ' || c == ',' || c == ':' || c < 32 || c == 127)
			return (false);
	}
	return (true);
}

//	Envoie line a tous les membres, sauf exclude si il est donne
static void broadcast(t_replies &out, const Channel &chan, const std::string &line, const User *exclude = NULL)
{
	const std::vector <User *> &members = chan.getMember();

	for (size_t i = 0; i < members.size(); ++i)
	{
		if (members[i] != exclude)
		{
			t_reply reply;
			reply.target = members[i];
			reply.line = line;
			out.push_back(reply);
		}
	}
}

//	Envoie line a un seul user (reponses numeriques : erreurs, 324, 353...)
static void reply(t_replies &out, User *target, const std::string &line)
{
	t_reply r;
	r.target = target;
	r.line = line;
	out.push_back(r);
}

//	Entier strictement positif, 9 chiffres maximum pour eviter tout depassement
static bool parseLimit(const std::string &str, size_t &limit)
{
	if (str.empty() || str.size() > 9)
		return (false);
	for (size_t i = 0; i < str.size(); ++i)
		if (!std::isdigit(static_cast <unsigned char>(str[i])))
			return (false);
	std::istringstream stream(str);
	stream >> limit;
	return (limit > 0);
}

static bool isValidKey(const std::string &key)
{
	return (!key.empty() && key[0] != ':' && key.find_first_of(" ,") == std::string::npos);
}

//	Note un changement pour la ligne MODE finale, par exemple "+it-k"
static void constructMode(std::string &applied, char &lastSign, char sign, char mode)
{
	if (lastSign != sign)
		applied += sign;
	lastSign = sign;
	applied += mode;
}

//	Lettres des modes actifs pour 324 RPL_CHANNELMODEIS, par exemple "+itkl"
static std::string modeString(const Channel &chan)
{
	const t_modes modes = chan.getMode();
	std::string str = "+";

	if (modes.inviteOnly)
		str += 'i';
	if (modes.topicLocked)
		str += 't';
	if (!modes.key.empty())
		str += 'k';
	if (modes.limit != 0)
		str += 'l';
	return (str);
}

//	Arguments des modes actifs, dans le meme ordre que modeString : " cle 10".
//	La key n'est montree qu'aux membres, les autres voient "*"
static std::string modeArgs(const Channel &chan, const User *asker)
{
	const t_modes modes = chan.getMode();
	std::string args;

	if (!modes.key.empty())
		args += " " + (chan.isMember(asker) ? modes.key : std::string("*"));
	if (modes.limit != 0)
	{
		std::ostringstream stream;
		stream << modes.limit;
		args += " " + stream.str();
	}
	return (args);
}

//	Prefix IRC de l'expediteur : "nick!user@host"
static std::string prefix(User &user)
{
	return (user.getNickname() + '!' + user.getUsername() + "@localhost");
}

//	Constructors

ChannelCommands::ChannelCommands() : _channels() {}

ChannelCommands::~ChannelCommands() {}

ChannelCommands::ChannelCommands(const ChannelCommands &cpy) : _channels(cpy._channels) {}

ChannelCommands &ChannelCommands::operator=(const ChannelCommands &cpy)
{
	if (this != &cpy)
		this->_channels = cpy._channels;
	return (*this);
}

Channel *ChannelCommands::getChannel(const std::string &name)
{
	std::map <std::string, Channel>::iterator it = this->_channels.find(toLower(name));

	if (it == this->_channels.end())
		return (NULL);
	return (&it->second);
}

//	A appeler apres chaque retrait d'un membre.
void ChannelCommands::deleteIfEmpty(const std::string &name)
{
	const Channel *chan = this->getChannel(name);

	if (chan && chan->isChannelEmpty())
		this->_channels.erase(toLower(name));
}

//	JOIN <channel>{,<channel>} [<key>{,<key>}]
//	Decoupe la liste des chnnels et celle des clefs, puis apelle joinOne pour chaque channel.
t_replies ChannelCommands::join(User &sender, const t_params &params)
{
	t_replies out;

	if (params.empty() || params[0].empty())
	{
		reply(out, &sender, err::needMoreParams(sender.getNickname(), "JOIN"));
		return (out);
	}
	const std::vector <std::string> channels = split(params[0], ',');
	std::vector <std::string> keys;
	if (params.size() > 1)
		keys = split(params[1], ',');
	for (size_t i = 0; i < channels.size(); ++i)
		if (!channels[i].empty())
			this->joinOne(sender, channels[i], i < keys.size() ? keys[i] : "", out);
	return (out);
}

//	joinOne et join sont separees car JOIN peut viser plusieurs channels
//	(echec sur 1 ne doit pas impacter les autres)
void ChannelCommands::joinOne(User &sender, const std::string &name, const std::string &key, t_replies &out)
{
	const std::string &nick = sender.getNickname();

	if (!isValidChannelName(name))
		return (reply(out, &sender, err::noSuchChannel(nick, name)));
	Channel *chan = this->getChannel(name);
	if (!chan)
	{
		chan = &this->_channels.insert(std::make_pair(toLower(name), Channel(name, &sender))).first->second;
	}
	else
	{
		const t_modes modes = chan->getMode();
		if (chan->isMember(&sender))
			return ;
		if (!modes.key.empty() && modes.key != key)
			return (reply(out, &sender, err::badChannelKey(nick, chan->getName())));
		if (modes.inviteOnly && !chan->isGuest(&sender))
			return (reply(out, &sender, err::inviteOnlyChan(nick, chan->getName())));
		if (modes.limit != 0 && chan->membersNb() >= modes.limit)
			return (reply(out, &sender, err::channelIsFull(nick, chan->getName())));
		chan->addMember(&sender);
	}
	sender.addChannel(chan);
	broadcast(out, *chan, ":" + prefix(sender) + " JOIN " + chan->getName() + "\r\n");
	//	Au JOIN, pas de 331 si le topic est vide : on n'envoie 332 que s'il existe
	if (!chan->getTopic().empty())
		reply(out, &sender, rpl::topic(nick, chan->getName(), chan->getTopic()));
	const std::vector <std::string> lists = this->names(chan->getName());
	for (size_t i = 0; i < lists.size(); ++i)
		reply(out, &sender, rpl::namReply(nick, chan->getName(), lists[i]));
	reply(out, &sender, rpl::endOfNames(nick, chan->getName()));
}

//	PART <channel>{,<channel>} [:<raison>]

t_replies ChannelCommands::part(User &sender, const t_params &params)
{
	t_replies out;

	if (params.empty() || params[0].empty())
	{
		reply(out, &sender, err::needMoreParams(sender.getNickname(), "PART"));
		return (out);
	}
	const std::vector <std::string> channels = split(params[0], ',');
	for (size_t i = 0; i < channels.size(); ++i)
		if (!channels[i].empty())
			this->partOne(sender, channels[i], params.size() > 1 ? params[1] : "", out);
	return (out);
}

void ChannelCommands::partOne(User &sender, const std::string &name, const std::string &reason, t_replies &out)
{
	Channel *chan = this->getChannel(name);

	if (!chan)
		return (reply(out, &sender, err::noSuchChannel(sender.getNickname(), name)));
	if (!chan->isMember(&sender))
		return (reply(out, &sender, err::notOnChannel(sender.getNickname(), chan->getName())));
	std::string line = ":" + prefix(sender) + " PART " + chan->getName();
	if (!reason.empty())
		line += " :" + reason;
	//	Broadcast avant le retrait, pour que celui qui part recoive aussi le message
	broadcast(out, *chan, line + "\r\n");
	chan->rmMember(&sender);
	sender.removeChannel(chan);
	this->deleteIfEmpty(name);
}

//	NAMES : les reponses 353 et 366 seront construites avec ce resultat

std::vector <std::string> ChannelCommands::names(const std::string &channel)
{
	std::vector <std::string> lists;
	const Channel *chan = this->getChannel(channel);

	if (!chan)
		return (lists);
	const std::vector <User *> &members = chan->getMember();
	std::string list;
	for (size_t i = 0; i < members.size(); ++i)
	{
		std::string entry = (chan->isOperator(members[i]) ? "@" : "") + members[i]->getNickname();
		if (!list.empty() && list.size() + entry.size() + 1 > 400)
		{
			lists.push_back(list);
			list.clear();
		}
		list += (list.empty() ? "" : " ") + entry;
	}
	if (!list.empty())
		lists.push_back(list);
	return (lists);
}

//	PRIVMSG <channel> :<texte>

t_replies ChannelCommands::privmsg(User &sender, const std::string &target, const std::string &text)
{
	t_replies out;
	const Channel *chan = this->getChannel(target);

	if (text.empty())
		reply(out, &sender, err::noTextToSend(sender.getNickname()));
	else if (!chan)
		reply(out, &sender, err::noSuchNick(sender.getNickname(), target));
	else if (!chan->isMember(&sender))
		reply(out, &sender, err::cannotSendToChan(sender.getNickname(), chan->getName()));
	else
		broadcast(out, *chan, ":" + prefix(sender) + " PRIVMSG " + chan->getName() + " :" + text + "\r\n", &sender);
	return (out);
}

//	MODE <channel> [<modes> [<arguments>...]]

t_replies ChannelCommands::mode(User &sender, const t_params &params)
{
	t_replies out;
	const std::string &nick = sender.getNickname();

	if (params.empty())
	{
		reply(out, &sender, err::needMoreParams(nick, "MODE"));
		return (out);
	}
	//	Les modes de user ("MODE alice +i", envoye par les clients a la connexion) ne sont
	//	pas demandes par le sujet : on les ignore
	if (!isChannelName(params[0]))
		return (out);
	Channel *chan = this->getChannel(params[0]);
	if (!chan)
	{
		reply(out, &sender, err::noSuchChannel(nick, params[0]));
		return (out);
	}
	if (params.size() == 1)
	{
		reply(out, &sender, rpl::channelModeIs(nick, chan->getName(), modeString(*chan), modeArgs(*chan, &sender)));
		return (out);
	}
	//	Certains clients demandent la liste des bans en rejoignant, sans etre operateurs
	if (params[1] == "b" || params[1] == "+b")
	{
		reply(out, &sender, rpl::endOfBanList(nick, chan->getName()));
		return (out);
	}
	if (!chan->isOperator(&sender))
	{
		reply(out, &sender, err::chanOPrivsNeeded(nick, chan->getName()));
		return (out);
	}

	const std::string &letters = params[1];
	size_t argIndex = 2;		//	Prochain argument a consommer
	char sign = '+';
	char lastSign = 0;
	std::string applied;		//	Lettres vraiment appliquees : "+it-k"
	std::string appliedArgs;	//	Leurs arguments : " cle bob"

	for (size_t i = 0; i < letters.size(); ++i)
	{
		const char letter = letters[i];
		const bool adding = (sign == '+');
		const t_modes modes = chan->getMode();

		if (letter == '+' || letter == '-')
			sign = letter;
		else if (letter == 'i')
		{
			if (modes.inviteOnly != adding)
			{
				chan->setInviteOnly(adding);
				constructMode(applied, lastSign, sign, letter);
			}
		}
		else if (letter == 't')
		{
			if (modes.topicLocked != adding)
			{
				chan->setTopicLocked(adding);
				constructMode(applied, lastSign, sign, letter);
			}
		}
		else if (letter == 'k' && adding)
		{
			if (argIndex >= params.size())
			{
				reply(out, &sender, err::needMoreParams(nick, "MODE"));
				continue ;
			}
			const std::string &key = params[argIndex++];
			if (!isValidKey(key))
			{
				reply(out, &sender, err::invalidModeParam(nick, chan->getName(), letter, key, "Invalid key"));
				continue ;
			}
			chan->setKey(key);
			constructMode(applied, lastSign, sign, letter);
			appliedArgs += " " + key;
		}
		else if (letter == 'k')
		{
			//	"-k" s'ecrit avec ou sans la key : on consomme l'argument s'il y en a un
			if (argIndex < params.size())
				++argIndex;
			if (!modes.key.empty())
			{
				chan->setKey("");
				constructMode(applied, lastSign, sign, letter);
				appliedArgs += " *";
			}
		}
		else if (letter == 'l' && adding)
		{
			size_t limit = 0;
			if (argIndex >= params.size())
			{
				reply(out, &sender, err::needMoreParams(nick, "MODE"));
				continue ;
			}
			const std::string &arg = params[argIndex++];
			if (!parseLimit(arg, limit))
			{
				reply(out, &sender, err::invalidModeParam(nick, chan->getName(), letter, arg, "Invalid limit"));
				continue ;
			}
			chan->setLimit(limit);
			constructMode(applied, lastSign, sign, letter);
			appliedArgs += " " + arg;
		}
		else if (letter == 'l')
		{
			if (modes.limit != 0)
			{
				chan->setLimit(0);
				constructMode(applied, lastSign, sign, letter);
			}
		}
		else if (letter == 'o')
		{
			if (argIndex >= params.size())
			{
				reply(out, &sender, err::needMoreParams(nick, "MODE"));
				continue ;
			}
			const std::string &targetNick = params[argIndex++];
			User *target = chan->getMemberByNick(targetNick);
			if (!chan->isMember(target))
			{
				reply(out, &sender, err::userNotInChannel(nick, targetNick, chan->getName()));
				continue ;
			}
			if (chan->isOperator(target) != adding)
			{
				if (adding)
					chan->addOperator(target);
				else
					chan->rmOperator(target);
				constructMode(applied, lastSign, sign, letter);
				appliedArgs += " " + target->getNickname();
			}
		}
		else
			reply(out, &sender, err::unknownMode(nick, letter));
	}
	if (!applied.empty())
		broadcast(out, *chan, ":" + prefix(sender) + " MODE " + chan->getName() + " " + applied + appliedArgs + "\r\n");
	return (out);
}
