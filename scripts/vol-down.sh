#!/bin/bash

# /usr/bin/amixer -qM set Master 5%- umute
pactl set-sink-volume @DEFAULT_SINK@ -5%
#sh ~/scripts/dwm-status-refresh.sh
echo "down"
