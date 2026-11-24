__This project has been created as part of the 42 curriculum by **aandreo**, **thlibers** and **nclavel**.__

<div align="center">
	<h1>ft_irc</h1>
	<p><b>A simple IRC server coded from scratch in C++98</b></p>
	<p>
		<img alt="language" src="https://img.shields.io/badge/language-C%2B%2B98-blue">
		<img alt="platform" src="https://img.shields.io/badge/platform-Linux-lightgrey">
		<img alt="status" src="https://img.shields.io/badge/status-work%20in%20progress-orange">
		<a href="./docs"><img alt="documentation" src="https://img.shields.io/badge/documentations-4468a6"></a>
	</p>
</div>

<div align="center">
	<h2>Requirement</h2>
</div>

- Linux
- A C++ compiler supporting C++98 (`c++`, `g++` or `clang++`)
- GNU Make

<div align="center">
	<h2>Build</h2>
</div>

1. Clone the project repository
```sh
git clone https://github.com/thlibers/irc.git && cd irc
```
2. Compile the project using **GNU Make**
```sh
make
```

<div align="center">
	<h2>Usage</h2>
</div>

Launch the server with
```sh
./ircserv <port> <password>
# e.g
./ircserv 6667 "really_hard_password"
```

- `<port>` must be a number between **1 and 65535**;
- `<password>` is the connection password, up to **1024 characters**.

<div align="center">
	<h2>Supported commands</h2>
</div>


Once connected and authenticated with `PASS`, `NICK` and `USER`, the following commands are planned:

| Command | Description |
| --- | --- |
| `PASS` | Authenticate with the server password |
| `NICK` | Set or change your nickname |
| `USER` | Register your username |
| `JOIN` | Join (or create) a channel |
| `PART` | Leave a channel |
| `PRIVMSG` | Send a message to a user or a channel |
| `NOTICE` | Send a notice to a user or a channel |
| `KICK` | *(channel operators)* Remove a user from a channel |
| `INVITE` | *(channel operators)* Invite a user to a channel |
| `TOPIC` | Get or change a channel's topic |
| `MODE` | Change channel modes: `i`, `t`, `k`, `o`, `l` |
| `NAMES` | List the users of a channel |
| `PING` / `PONG` | Keep-alive between client and server |
| `QUIT` | Disconnect from the server |

<div align="center">
	<h2>Makefile targets</h2>
</div>

| Target | Description | Command |
| --- | --- | --- |
| `all` | Compile the program (default target) | `make` |
| `ircserv` | Same as `all` | `make ircserv` |
| `clean` | Remove every object file (`.o`) | `make clean` |
| `fclean` | Remove both object files and the binary | `make fclean` |
| `re` | Run `fclean` then recompile the program | `make re` |

> [!NOTE]
> By default the server is compiled with the **`-O2`** flag, which tells the compiler to optimize when it can. Setting the **`DEBUG`** make variable (e.g. `make re DEBUG=1`) compiles with **`-g3`** instead of **`-O2`**, defines the `DEBUG` macro and enables debug output at runtime.

<div align="center">
	<h2>References</h2>
</div>

- [Modern IRC Client Protocol](https://modern.ircdocs.horse/)
- [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459) · [RFC 2812](https://datatracker.ietf.org/doc/html/rfc2812)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [man poll](https://man7.org/linux/man-pages/man2/poll.2.html)
