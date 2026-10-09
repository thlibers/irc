
#pragma once

class User;
class Message;

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
