#!/bin/bash

# PID="$(pgrep irc)"
PORT="6667"
PASSWORD="password"

if [ "$(ls -l | grep "ircserv")" == "" ]; then
	echo "ircserv binary not found in your cwd"
	exit 1
fi

if [ "$1" == "" ] || [ "$2" == "" ] || [ "$3" == "" ]; then
	printf "Usage: $0 [username] [real name] [nickname]\n"
	exit 1
fi

./ircserv $PORT $PASSWORD >/dev/null &

# sleep 1

echo -e "USER $1 0 * $2\r\nNICK $3\r\nPASS $PASSWORD" | nc -C 127.0.0.1 $PORT

# kill -SIGINT $(jobs | tail -n 1)
