
<div align="center">
	<h1>COMMAND</h1>
</div>

This documentation explains how a command sent by a client is treated, from the parsing (how we split a line into many parts to extract elements like the command, every parameter or the text message) to the execution (how we launch the correct function), and how the numeric reply<sup>5</sup> of the server is built and sent back.

<div align="center">
	<h1>SUMMARY</h1>
</div>

- [PARSING](#parsing)
	- [PARSING THE MESSAGE CLASS](#parsing-the-message-class)
	- [PARSING SPECIAL CASES](#parsing-special-cases)
- [DISPATCHING](#dispatching)
	- [DISPATCHING THE HANDLER MAP](#dispatching-the-handler-map)
	- [DISPATCHING REGISTRATION GUARD](#dispatching-registration-guard)
- [EXECUTION](#execution)
	- [EXECUTION THE ACOMMAND HIERARCHY](#execution-the-acommand-hierarchy)
	- [EXECUTION ERRORS](#execution-errors)
- [REPLIES](#replies)
	- [REPLIES NUMERIC CODES](#replies-numeric-codes)
	- [REPLIES BUILDERS](#replies-builders)
- [GLOSSARY](#glossary)

<div align="center">
	<h1>PARSING</h1>
</div>

Once `Server::Receive()` has cut the reception buffer into complete `\r\n` lines, each line is wrapped in a `Message` object<sup>1</sup>, which extracts the command, every parameter and the text message.

<!-- TODO: the IRC message format and its limits (512 bytes, 15 parameters max) -->

<div align="center">
	<h2>PARSING THE MESSAGE CLASS</h2>
</div>

<!-- TODO: the Message constructors + parse(): which part of the line fills which attribute (_command, _parameter, _text_message, _raw_message) -->

<div align="center">
	<h2>PARSING SPECIAL CASES</h2>
</div>

<!-- TODO: comma separated parameters (_parseCommaSeparatedParams), the ":" trailing parameter<sup>4</sup>, the empty line -->

<div align="center">
	<h1>DISPATCHING</h1>
</div>

Once the line is parsed, the `Dispatcher` looks up the command name in its handler map<sup>2</sup> and calls the handler<sup>3</sup> associated with it.

<!-- TODO: how executeCommand() finds the right handler, command names being case insensitive -->

<div align="center">
	<h2>DISPATCHING THE HANDLER MAP</h2>
</div>

<!-- TODO: the _handler map (std::map<std::string, Handler>) and what happens when the command is unknown (ERR_UNKNOWNCOMMAND 421) -->

<div align="center">
	<h2>DISPATCHING REGISTRATION GUARD</h2>
</div>

<!-- TODO: PASS / NICK / USER must be received before anything else, otherwise ERR_NOTREGISTERED (451) -->

<div align="center">
	<h1>EXECUTION</h1>
</div>

The handler runs and returns a `uint16_t` numeric code<sup>5</sup> telling the server whether the command succeeded.

<!-- TODO: what a handler does step by step (check parameters, check rights, apply the effect, queue the replies) -->

<div align="center">
	<h2>EXECUTION THE ACOMMAND HIERARCHY</h2>
</div>

<!-- TODO: the ACommand base class (user + message, run() = 0) and the classes inheriting from it: Pass, Nick, User, Privmsg -->

<div align="center">
	<h2>EXECUTION ERRORS</h2>
</div>

<!-- TODO: the FailedExec exception (code + formatted message) and how the dispatcher turns it into a numeric reply -->

<div align="center">
	<h1>REPLIES</h1>
</div>

The server never answers with plain text: it always sends a numeric reply, built as one ready to send line.

<!-- TODO: how a reply travels from the handler to the send buffer (_sendbuf + POLLOUT) -->

<div align="center">
	<h2>REPLIES NUMERIC CODES</h2>
</div>

<!-- TODO: irc.hpp, every RPL_ / ERR_ code as a #define -->

<div align="center">
	<h2>REPLIES BUILDERS</h2>
</div>

<!-- TODO: Replies.hpp, the rpl:: and err:: namespaces, the build() format ":ircserv <code> <nick> <params>\r\n" -->

<div align="center">
	<h1>GLOSSARY</h1>
</div>

<!-- An entry stays short: one or two lines maximum. If an explanation needs more, it probably deserves its own section in the body. -->
- Message<sup>1</sup>: The class that receives a raw line and cuts it into the command, the parameters and the text message.
- map<sup>2</sup>: A container that works like a drawer cabinet: give it a key (here a command name) and it hands back the value stored with it (here the function to call).
- handler<sup>3</sup>: The function launched when its command arrives, in charge of checking it and applying its effect.
- trailing parameter<sup>4</sup>: The last parameter of a message, written after a ":" so it can contain spaces (the body of a PRIVMSG, a topic...).
- numeric reply<sup>5</sup>: The 3 digits code the server sends instead of a plain sentence, so every client understands it the same way (001 = welcome, 433 = nickname already used...).
