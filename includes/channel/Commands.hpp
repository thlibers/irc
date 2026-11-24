#pragma once

#include "includes/channel/Channel.hpp"
#include <map>
#include <string>
#include <vector>

class User;

//	Un message a envoyer. Les commandes n'envoient rien elles-memes (send() reste dans la
//	boucle poll()) : le serveur ajoute line au buffer d'envoi de target.
typedef struct s_reply {
	User			*target;	//	Destinataire
	std::string		line;		//	Ligne complete, "\r\n" compris
}	t_reply;

typedef std::vector <t_reply>		t_replies;
typedef std::vector <std::string>	t_params;

//	Possede tous les channels du serveur et execute les commandes qui les concernent.
//	params contient tout ce qui suit le nom de la commande, le texte apres ":" en dernier.
class ChannelCommands
{
private :
	std::map <std::string, Channel>	_channels;	//	Cle : nom en minuscules, "#TEst" et "#test" sont le meme channel

	void joinOne(User &sender, const std::string &name, const std::string &key, t_replies &out);
	void partOne(User &sender, const std::string &name, const std::string &reason, t_replies &out);

public :
	ChannelCommands();
	~ChannelCommands();
	ChannelCommands(const ChannelCommands &cpy);
	ChannelCommands &operator=(const ChannelCommands &cpy);

	Channel *getChannel(const std::string &name);
	void deleteIfEmpty(const std::string &name);

	t_replies join(User &sender, const t_params &params);
	t_replies part(User &sender, const t_params &params);
	t_replies mode(User &sender, const t_params &params);
	t_replies privmsg(User &sender, const std::string &target, const std::string &text);
	std::vector <std::string> names(const std::string &channel);
};
