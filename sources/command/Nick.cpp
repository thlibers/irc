
#include "includes/command/Nick.hpp"
#include "includes/command/ACommande.hpp"
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/namespace/helper.hpp"

#include <stdexcept>
#include <vector>

Nick::Nick(void) : ACommand(), _new_nickname("") { ; }
Nick::Nick(User &user, Message &message) : ACommand(user, message)
{ ; } // TODO
Nick::Nick(Nick &cpy) : ACommand(cpy), _new_nickname(cpy._new_nickname) { ; }
Nick::~Nick(void) { ; }

void	Nick::run(void)
{
	;
}
