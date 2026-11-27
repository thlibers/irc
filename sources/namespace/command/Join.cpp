
#include "includes/namespace/Replies.hpp"
#include "includes/namespace/commands.hpp"


command::Join::Join(::User &user, const ::Message &message) :
	ACommand(user, message)
{ ; }
command::Join::~Join(void)
{ ; }

// Wrapper
replies::rtn_i_t	command::Join::run(void)
{
	return (replies::rtn_i_t());
}
