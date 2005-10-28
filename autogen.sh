#!/bin/bash

echo "Regenerating autoconf/automake stuff.."

if [ ! -e "aclocal.m4" ]; then 
	echo "-- aclocal.."
	aclocal
else 
	echo "-- skipping aclocal.."
fi

echo "-- autoheader.. " && 		autoheader && \
echo "-- automake.." && 				automake --add-missing --copy && \
echo "-- autoconf.." && 				autoconf && \
echo "Done."
