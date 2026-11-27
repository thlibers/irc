
#pragma once
#include <inttypes.h>
#include <string>
#include <vector>

class Channel;
typedef std::vector<Channel *>::const_iterator channel_const_it;
typedef std::vector<Channel *>::iterator channel_it;

/*
 * @brief: This class will contain every user related data
 * @detail: On this class we will store generic data such as the username,
 *     the nickname, the real name. But also a vector "Channel" class pointer
 *     who represent every channel that the user have joined.
 */
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
		// The fd of the client socket linked to this user, -1 until set
		int						_fd;
		// True while the client is connected, set to false by the server
		// before dropping his connection (only used by QUIT)
		bool					_alive;
		// A vector with every channel that the user is on
		std::vector<Channel *>	_channel_joined;
		// 8bits that define if a user is registerd or not
		uint8_t					_registered_flags;

	public:
		// --- ENUMS ---
		typedef enum e_register_status
		{
			UNREGISTER = 0,
			PASSWD_SUPPLIED = 0b1,
			NICK_SUPPLIED = 0b10,
			USR_SUPPLIED = 0b100,
			REGISTER = PASSWD_SUPPLIED | NICK_SUPPLIED | USR_SUPPLIED

		}			 t_register_status;

		// --- CONSTRUCTOR / DESTRUCTOR ---
		User(void);
		User(const std::string &nickname, const std::string &username, const std::string &real_name);
		~User(void);

		// --- SETTER ---

		void	setNickname(const std::string &nickname) throw();
		void	setUsername(const std::string &username) throw();
		void	setRealName(const std::string &realname) throw();
		void	setFd(int fd) throw();
		void	setAlive(bool alive) throw();

		// --- GETTER ---

		const std::string	&getNickname(void) const throw();
		const std::string	&getUsername(void) const throw();
		const std::string	&getRealName(void) const throw();
		int					getFd(void) const throw();
		bool				isAlive(void) const throw();

		// --- USER CHANNEL ---

		bool	isOnChannel(Channel &channel) const throw();
		bool	isOnChannel(std::string &channel) const throw();
		bool	addChannel(Channel &channel) throw();
		bool	addChannel(Channel *channel) throw();
		bool	removeChannel(Channel &channel) throw();
		bool	removeChannel(Channel *channel) throw();
		Channel *getChannelByName(std::string &channel_name) const throw();

		// --- USER COMMAND ---
		uint8_t	isRegisted(void) const throw();
		void	setFlag(uint8_t flag) throw();

		// --- Wrapper ---
		static User	*createNewUser(std::string &nickname,
								   std::string &username,
								   std::string &realname);
};

typedef std::vector<User*>::iterator		user_it;
typedef std::vector<User*>::const_iterator	user_const_it;
