
#pragma once

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <inttypes.h>
#include <string>
#include <map>
#include <vector>

#include "includes/namespace/Replies.hpp"

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

			/*
			 * @brief: Build a return info filled with the fresh identity of the
			 *         sender at the exact moment of the return: the errors happen
			 *         before any change, the success is reported after them
			 * @param: code, the IRC code to answer, 0 = nothing to send
			 * @param: target, the faulty nick / channel when there is one
			 * @return: a t_return_info ready to be given to replies::build
			 */
			replies::t_return_info	_makeReturnInfo(uint16_t code, const std::string &target = "") const;
		public:
			ACommand(::User &user, const ::Message &message);

			virtual ~ACommand(void) = 0;
			virtual replies::t_return_info run(void) = 0;

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
			const std::map<int, ::User *>	&_user_map;
		public:
			Nick(::User &user, const ::Message &message, const std::map<int, ::User *> &user_map);
			Nick(Nick &cpy);
			~Nick(void);

			replies::t_return_info	run(void);
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

			replies::t_return_info	run(void);
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

			replies::t_return_info	run(void);
	};
	/*
	 * @brief:  This class is the representation of the PRIVMSG command
	 * @detail: It inherit from ACommand pure abstract class
	 */
	class Privmsg : public ACommand
	{
		private:
			// The map with every connected user, used to check that the
			// target of the message exists (ERR_NOSUCHNICK)
			std::map<int, ::User *>		&_user_map;
			// The first target given by the sender (channel or nick)
			std::string					_channel;
			// Every remaining target, only the first one is handled for now
			std::vector<std::string>	_userlist;
		public:
			Privmsg(::User &user, const ::Message &message, std::map<int, ::User *> &user_map);
			Privmsg(Privmsg &cpy);
			~Privmsg(void);

			replies::t_return_info	run(void);

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

	class Ping : public ACommand
	{
		public:
			Ping(::User &user, const ::Message &message);
			~Ping(void);

			replies::rtn_i_t	run(void);
	};

	class Quit : public ACommand
	{
		public:
			Quit(::User &user, const ::Message &message);
			~Quit(void);

			replies::rtn_i_t	run(void);
	};
	class Join : public ACommand
	{
		public:
			Join(::User &user, const ::Message &message);
			~Join(void);

			replies::rtn_i_t	run(void);
	};

	//TEMPLATE CTRL+C ; CTRL+V
// 	class UNSET : public ACommand
// 	{
// 		public:
// 			UNSET(::User &user, const ::Message &message);
// 			~UNSET(void);
//
// 			replies::rtn_i_t	run(void);
// 	};
}
