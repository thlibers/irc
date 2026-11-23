
<div align="center">
	<h1>CLI</h1>
</div>

A CLI (also know as Command Line Interface) is a way of interacting with a program by typing text commands, instead of clicking buttons in a graphical interface (a GUI) or moving a cursor in a terminal interface (TUI). The adventage is that the CLI is easy to implement by just getting arguments passed in the process arguments (**argv**).

The **CLI** on ft_irc is a **Class** called "**Cli**". The class is instantiated at the very start of `main()` with the process arguments, and acts as the **entry point** of the server: it validates what the user typed on the command line, and (once the `Server` class is ready) launches the server with the validated values.

Everything the CLI does happens **before** any network activity: if the arguments are wrong, the program prints an error and exits with code **1** without ever opening a socket.

<div align="center">
	<h2>Usage</h2>
</div>

```sh
./ircserv <port> <password>
# e.g
./ircserv 6667 "really_hard_password"
```

| Argument | Type | Constraints |
| --- | --- | --- |
| `<port>` | `uint16_t` | Digits only, between **1** and **65535** (port `0` is refused) |
| `<password>` | `std::string` | Up to **1024 characters**, this is the password clients must send with `PASS` |

Giving fewer than two arguments, or more than two, is an error.

<div align="center">
	<h2>Program flow</h2>
</div>

```
main()                          sources/main.cpp
  │
  ├─ Cli cli(argc, argv);       build the Cli from the process arguments
  │
  ├─ cli.run();                 validate the arguments (may throw)
  │     │
  │     ├─ helper::checkPort()      checks argv[1]
  │     └─ helper::checkPassword()  checks argv[2]
  │
  ├─ catch (Cli::Cli_error)     print the error, call cli.help(), exit 1
  ├─ catch (std::exception)     print the error, exit 1
  │
  └─ return 0;                  arguments were valid
```

The `Cli` class deliberately **does not** own the error printing: `run()` throws and `main()` decides how to display the problem. This keeps the CLI a pure *validator / launcher*.

<div align="center">
	<h2>The Cli class</h2>
</div>

Declared in [`includes/Cli.hpp`](../includes/Cli.hpp), implemented in [`sources/Cli.cpp`](../sources/Cli.cpp).

**Private members**

| Member | Type | Role |
| --- | --- | --- |
| `_server` | `Server *` | The server instance, `NULL` until the server is launched |
| `_argc` | `int` | The argument count, as given by `main()` |
| `_argv` | `char **` | The argument array, as given by `main()` |

**Public members**

| Member | Role |
| --- | --- |
| `Cli(void)` | Default constructor, everything is `NULL`/`0` |
| `Cli(int &argc, char **&argv)` | Main constructor, stores the process arguments |
| `Cli(Cli &cpy)` | Copy constructor, copies `_argc` and `_argv` (shallow) |
| `operator=(Cli &cpy)` | Assignment operator, guards against self-assignment |
| `~Cli(void)` | Destructor, will `delete` the server once it is owned |
| `run()` | Validates the arguments and launches the server |
| `help()` | Prints the usage message |

> [!NOTE]
> The class holds a forward-declared `Server *` (`class Server;`) instead of including the server header. This keeps `Cli.hpp` independent from the `Server` implementation.

<div align="center">
	<h2>run()</h2>
</div>

`run()` is the heart of the CLI. It performs, in order:

1. **Argument count check** — exactly two arguments are expected (plus the program name), otherwise a `Cli_error` is thrown;
2. **Port validation** — `argv[1]` is checked by `helper::checkPort()`;
3. **Password validation** — `argv[2]` is checked by `helper::checkPassword()`;
4. **Server launch** — *planned*: create the `Server` with `new Server(port, password)` and run it (currently commented out while the server is being developed).

If any step fails, `run()` throws and the error is handled by `main()`.

<div align="center">
	<h2>help()</h2>
</div>

`help()` is a small `throw()`-safe helper that prints the usage on **stderr**
It is called by `main()` right after a `Cli_error` is caught, so the user always sees *why* it failed **and** *how* to fix it.

<div align="center">
	<h2>Cli_error</h2>
</div>

`Cli_error` is an exception **nested inside the Cli class**. It inherits from `std::runtime_error` but keeps its own `_message` so `what()` returns exactly the message given at construction. It allow use to give a custom message on the exception easly:

| Member | Role |
| --- | --- |
| `Cli_error(void)` | Default constructor, message is `"undefined cli_error"` |
| `Cli_error(const std::string message)` | Builds the error with a custom message (usually a macro from `Error.hpp`) |
| `what()` | Returns the message as a C string |

The class is used for errors **the CLI itself detects** (wrong argument count). Errors detected by the `helper` functions are thrown as plain `std::runtime_error` and are caught by the generic `catch (std::exception &)` in `main()`.
