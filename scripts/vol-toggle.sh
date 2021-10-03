#!/bin/bash

# /usr/bin/amixer set Master toggle
pactl set-sink-mute 0 toggle
bash ~/scripts/dwm-status-refresh.sh
