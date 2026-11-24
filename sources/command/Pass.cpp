
#include "includes/command/Pass.hpp"

Pass::Pass(void) : ACommand(), _new_password("")
{ ; }
Pass::Pass(User &user, Message &message) : ACommand(user, message)
{ ; }
Pass::Pass(Pass &cpy) : ACommand(cpy), _new_password(cpy._new_password) { ; }
Pass::~Pass(void) { ; }

void	Pass::run(void)
{ ; }
