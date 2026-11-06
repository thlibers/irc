
#pragma once

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <inttypes.h>
#include <string>
class User;
class Message;

/*
 * @brief: This class is the base class of every commande
 * @detail: Pure virtual abstract class who will store the user who have sent the
 *     message and the message itself.
 */
class Command
{
	protected:
		User	*_user;
		Message *_message;
	public:
		Command(void);
		Command(User &user, Message &message);
		Command(Command &cpy);
		Command &operator=(Command &cpy);
		virtual ~Command(void) = 0;
		virtual void run(void) = 0;

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
