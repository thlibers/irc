
#pragma once
#include <string>
#include <vector>

class Channel;

class User
{
	private:
		// --- ATTRIBUTES ---

		// The nickname of the user (should be unique)
		std::string				_nickname;
		// The username of the user
		std::string				_username;
		// The real name of the user
		std::string				_real_name;
		// The password of the user
		std::string				_password;
		// The socket fd associated with the user, -1 if the user has no connection
		int						_fd;
		// Bit mask of the registration steps already validated.
		int						_reg_flags;
		// A vector with every channel that the user is on.
		// The User class does NOT own these channels (the server does),
		// it only keeps pointers to stay in sync with them.
		std::vector<Channel *>	_channel_list;

	public:
		/*
		 * Steps of the IRC registration handshake (PASS -> NICK -> USER).
		 * A user is fully registered only when every flag is set.
		 */
		typedef enum e_reg
		{
			REG_PASS = (1 << 0),	// The client sent a valid PASS command
			REG_NICK = (1 << 1),	// The client sent a valid NICK command
			REG_USER = (1 << 2),	// The client sent a valid USER command
			REG_ALL  = REG_PASS | REG_NICK | REG_USER
		}	t_reg;

		// --- CONSTRUCTOR / DESTRUCTOR ---

		User(void);
		explicit User(int fd);
		User(const std::string &nickname, const std::string &username,
			 const std::string &real_name, const std::string &password, int fd);
		User(const User &cpy);
		User &operator=(const User &cpy);
		~User(void);

		// --- SETTER ---

		void	setNickname(const std::string &nickname);
		void	setUsername(const std::string &username);
		void	setRealName(const std::string &realname);
		void	setPassword(const std::string &password);
		void	setFd(int fd);
		void	markRegStep(t_reg step);

		// --- GETTER ---

		const std::string	&getNickname(void) const;
		const std::string	&getUsername(void) const;
		const std::string	&getRealName(void) const;
		const std::string	&getPassword(void) const;
		int					getFd(void) const;

		// --- REGISTRATION STATE ---

		bool	isRegistered(void) const;

		// --- USER CHANNEL ---

		bool	isOnChannel(const Channel &channel) const;
		bool	isOnChannel(const std::string &channel_name) const;
		bool	addChannel(Channel &channel);
		bool	addChannel(Channel *channel);
		bool	removeChannel(const Channel &channel);
		bool	removeChannel(const Channel *channel);
		Channel	*getChannelByName(const std::string &channel_name) const;

		static User	*createNewUser(const std::string &nickname,
								   const std::string &username,
								   const std::string &realname,
								   const std::string &password,
								   int fd);
};

typedef std::vector<User*>::iterator		user_it;
typedef std::vector<User*>::const_iterator	user_const_it;
