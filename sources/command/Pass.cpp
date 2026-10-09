
#include "includes/command/Pass.hpp"
#include "includes/command/Commande.hpp"

Pass::Pass(void) : Command(), _new_password("")
{ ; }
Pass::Pass(User &user, Message &message) : Command(user, message)
{ ; }
Pass::Pass(Pass &cpy) : Command(cpy), _new_password(cpy._new_password) { ; }
Pass::~Pass(void) { ; }

void	run(void)
{ ; }
