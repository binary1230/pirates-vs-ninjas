#!/bin/bash

EXTRA_ACLOCAL_PATH="/opt/xmingw/i386-mingw32msvc/share/aclocal"

echo "Regenerating autoconf/automake stuff.."

# aclocal stuff with extra paths
# if [ ! -e "aclocal.m4" ]; then 
	echo "-- aclocal.."
	if [ -e "$EXTRA_ACLOCAL_PATH" ]; then
		echo "-- - Searching extra path: $EXTRA_ACLOCAL_PATH"
		aclocal -I "$EXTRA_ACLOCAL_PATH"
	else 
		echo "-- - Not searching extra path: $EXTRA_ACLOCAL_PATH"
		aclocal
	fi
#else 
#	echo "-- skipping aclocal.."
#fi

echo "-- autoheader.. " && 		autoheader && \
echo "-- automake.." && 				automake --add-missing --copy && \
echo "-- autoconf.." && 				autoconf && \
echo "Done."
