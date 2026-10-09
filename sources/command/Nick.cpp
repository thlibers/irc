
#include "includes/command/Nick.hpp"
#include "includes/command/Commande.hpp"
#include "includes/Message.hpp"
#include "includes/User.hpp"
#include "includes/namespace/helper.hpp"

#include <stdexcept>
#include <vector>

Nick::Nick(void) : Command(), _new_nickname("") { ; }
Nick::Nick(User &user, Message &message) : Command(user, message)
{ ; } // TODO
Nick::Nick(Nick &cpy) : Command(cpy), _new_nickname(cpy._new_nickname) { ; }
Nick::~Nick(void) { ; }

void	Nick::run(void)
{
	;
}
