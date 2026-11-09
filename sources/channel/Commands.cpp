#include "includes/channel/Commands.hpp"
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
static void broadcast(t_replies &out, const Channel &chan, const std::string &line, const std::string &exclude = "")
{
	const std::vector <std::string> members = chan.getMember();

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

t_replies ChannelCommands::join(const t_sender &sender, const t_params &params)
{
	t_replies out;

	if (params.empty() || params[0].empty())
		return (out);	//	461 ERR_NEEDMOREPARAMS
	const std::vector <std::string> channels = split(params[0], ',');
	std::vector <std::string> keys;
	if (params.size() > 1)
		keys = split(params[1], ',');
	for (size_t i = 0; i < channels.size(); ++i)
		if (!channels[i].empty())
			this->joinOne(sender, channels[i], i < keys.size() ? keys[i] : "", out);
	return (out);
}

void ChannelCommands::joinOne(const t_sender &sender, const std::string &name, const std::string &key, t_replies &out)
{
	if (!isValidChannelName(name))
		return ;	//	403 ERR_NOSUCHCHANNEL
	Channel *chan = this->getChannel(name);
	if (!chan)
	{
		chan = &this->_channels.insert(std::make_pair(toLower(name), Channel(name, sender.nick))).first->second;
	}
	else
	{
		const t_modes modes = chan->getMode();
		if (chan->isMember(sender.nick))
			return ;
		if (!modes.key.empty() && modes.key != key)
			return ;	//	475 ERR_BADCHANNELKEY
		if (modes.inviteOnly && !chan->isGuest(sender.nick))
			return ;	//	473 ERR_INVITEONLYCHAN
		if (modes.limit != 0 && chan->membersNb() >= modes.limit)
			return ;	//	471 ERR_CHANNELISFULL
		chan->addMember(sender.nick);
	}
	broadcast(out, *chan, ":" + sender.prefix + " JOIN " + chan->getName() + "\r\n");
	//	332 RPL_TOPIC si le channel a un topic, puis 353 RPL_NAMREPLY avec names() et 366 RPL_ENDOFNAMES
}

//	PART <channel>{,<channel>} [:<raison>]

t_replies ChannelCommands::part(const t_sender &sender, const t_params &params)
{
	t_replies out;

	if (params.empty() || params[0].empty())
		return (out);	//	461 ERR_NEEDMOREPARAMS
	const std::vector <std::string> channels = split(params[0], ',');
	for (size_t i = 0; i < channels.size(); ++i)
		if (!channels[i].empty())
			this->partOne(sender, channels[i], params.size() > 1 ? params[1] : "", out);
	return (out);
}

void ChannelCommands::partOne(const t_sender &sender, const std::string &name, const std::string &reason, t_replies &out)
{
	Channel *chan = this->getChannel(name);

	if (!chan)
		return ;	//	403 ERR_NOSUCHCHANNEL
	if (!chan->isMember(sender.nick))
		return ;	//	442 ERR_NOTONCHANNEL
	std::string line = ":" + sender.prefix + " PART " + chan->getName();
	if (!reason.empty())
		line += " :" + reason;
	//	Broadcast avant le retrait, pour que celui qui part recoive aussi le message
	broadcast(out, *chan, line + "\r\n");
	chan->rmMember(sender.nick);
	this->deleteIfEmpty(name);
}

//	NAMES : les reponses 353 et 366 seront construites avec ce resultat

std::vector <std::string> ChannelCommands::names(const std::string &channel)
{
	std::vector <std::string> lists;
	const Channel *chan = this->getChannel(channel);

	if (!chan)
		return (lists);
	const std::vector <std::string> members = chan->getMember();
	std::string list;
	for (size_t i = 0; i < members.size(); ++i)
	{
		std::string entry = (chan->isOperator(members[i]) ? "@" : "") + members[i];
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

t_replies ChannelCommands::privmsg(const t_sender &sender, const std::string &target, const std::string &text)
{
	t_replies out;
	const Channel *chan = this->getChannel(target);

	if (text.empty())
		return (out);	//	412 ERR_NOTEXTTOSEND
	if (!chan)
		return (out);	//	401 ERR_NOSUCHNICK
	if (!chan->isMember(sender.nick))
		return (out);	//	404 ERR_CANNOTSENDTOCHAN
	broadcast(out, *chan, ":" + sender.prefix + " PRIVMSG " + chan->getName() + " :" + text + "\r\n", sender.nick);
	return (out);
}

//	MODE <channel> [<modes> [<arguments>...]]

t_replies ChannelCommands::mode(const t_sender &sender, const t_params &params)
{
	t_replies out;

	if (params.empty())
		return (out);	//	461 ERR_NEEDMOREPARAMS
	//	Les modes de user ("MODE alice +i", envoye par les clients a la connexion) ne sont
	//	pas demandes par le sujet : on les ignore
	if (!isChannelName(params[0]))
		return (out);
	Channel *chan = this->getChannel(params[0]);
	if (!chan)
		return (out);	//	403 ERR_NOSUCHCHANNEL
	if (params.size() == 1)
		return (out);	//	324 RPL_CHANNELMODEIS, ne montrer la key qu'aux membres
	//	Certains clients demandent la liste des bans en rejoignant, sans etre operateurs
	if (params[1] == "b" || params[1] == "+b")
		return (out);	//	368 RPL_ENDOFBANLIST
	if (!chan->isOperator(sender.nick))
		return (out);	//	482 ERR_CHANOPRIVSNEEDED

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
				continue ;	//	461 ERR_NEEDMOREPARAMS
			const std::string &key = params[argIndex++];
			if (!isValidKey(key))
				continue ;	//	696 ERR_INVALIDMODEPARAM
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
				continue ;	//	461 ERR_NEEDMOREPARAMS
			const std::string &arg = params[argIndex++];
			if (!parseLimit(arg, limit))
				continue ;	//	696 ERR_INVALIDMODEPARAM
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
				continue ;	//	461 ERR_NEEDMOREPARAMS
			const std::string &target = params[argIndex++];
			if (!chan->isMember(target))
				continue ;	//	441 ERR_USERNOTINCHANNEL
			if (chan->isOperator(target) != adding)
			{
				if (adding)
					chan->addOperator(target);
				else
					chan->rmOperator(target);
				constructMode(applied, lastSign, sign, letter);
				appliedArgs += " " + target;
			}
		}
		//	sinon : 472 ERR_UNKNOWNMODE
	}
	if (!applied.empty())
		broadcast(out, *chan, ":" + sender.prefix + " MODE " + chan->getName() + " " + applied + appliedArgs + "\r\n");
	return (out);
}
