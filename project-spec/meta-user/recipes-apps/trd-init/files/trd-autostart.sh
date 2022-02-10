#!/bin/sh

# Execute userhook if present
USERHOOK_SD=/media/card/autostart.sh
if [ -f $USERHOOK_SD ]; then
	sh $USERHOOK_SD &
fi
