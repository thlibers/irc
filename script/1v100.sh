#!/bin/bash

PID="$(pgrep irc)"
PORT="6667"
PASSWORD="password"

#
if [ $(ls -l | grep "ircserv") -eq "" ]; then
	echo "ircserv binary not found in your cwd"
	exit 1
fi

# Kill every existing server
if [ $PID -ne "" ]; then
	kill -7 $PID
fi

./ircserv $PORT $PASSWORD &

for (i in {0..100}); do
	kill -INT $i
	$((i + 1))
done
