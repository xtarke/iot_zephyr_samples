#!/bin/sh

HOSTSPAWN="flatpak-spawn --directory=/tmp --host "

URL=$(git remote get-url origin)
if [ $? == 0 ]; then
	$HOSTSPAWN xdg-open $URL > /dev/null 2>&1 &
	exit 0
else
	echo "Get remote url failed !"
	exit 1
fi
