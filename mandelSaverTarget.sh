#!/bin/bash
pid=$(pgrep -u username nautilus)
dbus=$(grep -z DBUS_SESSION_BUS_ADDRESS /proc/$pid/environ | sed 's/DBUS_SESSION_BUS_ADDRESS=//' )
DBUS_SESSION_BUS_ADDRESS=$dbus

export PATH=/home/james/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
export DISPLAY=:0.0
cd /home/james/MandelSaver/build/
idle=$(xprintidle)
notify-send "Idle Time" $idle
if [[ $idle -ge 120000 ]]
then
    pkill -f MandelSaver
    ./MandelSaver
fi
