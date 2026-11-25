
#pragma once

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <inttypes.h>
#include <string>
#include <map>
#include <vector>

class User;
class Message;

/*
 * @brief: Every IRC command class (ACommand, Nick, Pass, Privmsg, User)
 *         live in the "command" namespace. See includes/command/.
 */
namespace command
{
	/*
	 * @brief: This class is the base class of every commande
	 * @detail: Pure virtual abstract class who will store the user who have sent the
	 *          message and the message itself.
	 */
	class ACommand
	{
		protected:
			::User			&_user;
			const ::Message	&_message;
		public:
			ACommand(::User &user, const ::Message &message);

			virtual ~ACommand(void) = 0;
			virtual uint16_t run(void) = 0;

			class FailedExec : public std::exception
		{
			private:
				uint16_t	_code;
				std::string	_msg;

			public:
				FailedExec(void) :
					_code(0),
					_msg("")
					{;}
				FailedExec(std::string msg) :
					_code(0),
					_msg(msg)
				{;}
				FailedExec(uint16_t code, std::string msg) :
					_code(code),
					_msg(msg)
				{;}
				FailedExec(uint16_t code, std::string msg, std::string format) :
					_code(code),
					_msg(msg)
				{
					(void)_code;
					std::size_t	form = msg.find("%s");
					if (form != std::string::npos)
						this->_msg.replace(form, 2, format);
				}
				~FailedExec(void) throw() {;}

				virtual const char *what(void) const throw()
				{
					return (_msg.c_str());
				}
		};
	};
	/*
	 * @brief:  This class is the representation of the NICK command
	 * @detail: It inherit from ACommand pure abstract class
	 */
	class Nick : public ACommand
	{
		private:
			std::map<int, ::User *>	&_user_map;
		public:
			Nick(::User &user, const ::Message &message, std::map<int, ::User *> &user_map);
			Nick(Nick &cpy);
			~Nick(void);

			uint16_t	run(void);
	};
	/*
	 * @brief:  This class is the representation of the PASS command
	 * @detail: It inherit from ACommand pure abstract class
	 */
	class Pass : public ACommand
	{
		private:
			std::string &_server_password;
		public:
			Pass(::User &user, const ::Message &message, std::string &server_password);
			~Pass(void);

			uint16_t	run(void);
	};
	/*
	 * @brief:  This class is the representation of the USER command
	 * @detail: It inherit from ACommand pure abstract class
	 */
	class User : public ACommand
	{
		public:
			User(::User &user, const ::Message &message);
			~User(void);

			uint16_t	run(void);
	};
	/*
	 * @brief:  This class is the representation of the USER command
	 * @detail: It inherit from ACommand pure abstract class
	 */
	class Privmsg : public ACommand
	{
		private:
			std::string					_channel;
			std::vector<std::string>	_userlist;
		public:
			Privmsg(::User &user, const ::Message &message);
			Privmsg(Privmsg &cpy);
			~Privmsg(void);

			uint16_t	run(void);

		// class FailedExec : public std::runtime_error
		// {
		// 	private:
		// 		std::string _message;
		// 	public:
		// 		FailedExec(void);
		// 		FailedExec(std::string messasge);
		// 		virtual ~FailedExec(void);
		// 		virtual const char	*what(void) const throw();
		// };
	};
}
