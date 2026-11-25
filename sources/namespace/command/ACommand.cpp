
#include "includes/namespace/commands.hpp"
#include <cstddef>

command::ACommand::ACommand(::User &user, const ::Message &message) : _user(user), _message(message) { ; }

command::ACommand::~ACommand(void) { ; }
