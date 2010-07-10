#!/bin/bash
# script to change the volume via acpi calls by joachim schiele 2010-06-30
# this ignores stereo since it only sets a common volume for both

if [ "$1"x == "up"x ]; then
	volume=$(amixer get Master | grep "Front Left: Playback" | awk '{print $4}')
	Y=$(echo  "$volume+1" | bc)
	amixer set Master $Y > /dev/null
	P=$(echo "$Y/30*100" | bc -l)
fi

if [ "$1"x == "down"x ]; then
	volume=$(amixer get Master | grep "Front Left: Playback" | awk '{print $4}')
	Y=$(echo  "$volume-1" | bc)
	amixer set Master $Y > /dev/null
	P=$(echo "$Y/30*100" | bc -l)
fi

if [ "$1"x == "mute"x ]; then
	output=$(amixer get Master | grep "Front Left: Playback" | grep '\[off\]')
	state=$?
	volume=$(amixer get Master | grep "Front Left: Playback" | awk '{print $4}')
	Y=$(echo  "$volume")
	amixer set Master $Y > /dev/null
	P=$(echo "$Y/30*100" | bc -l)

	#echo "state=$state"

	if [ $state -ne 1 ]; then
		amixer set Master unmute > /dev/null
	else
		amixer set Master mute > /dev/null
	fi
fi
