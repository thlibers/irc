
#pragma once

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
};
