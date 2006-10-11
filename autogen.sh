#!/bin/bash

# -----------------------------------------------------
# EDIT THIS if you have other aclocl macros somewhere
# (like your home dir)
# -----------------------------------------------------
EXTRA_ACLOCAL_PATH="/usr/local/share/aclocal"

# -----------------------------------------------------
# Don't edit below if you don't know what you're doing
# -----------------------------------------------------

echo "Regenerating autoconf/automake stuff.."

# aclocal stuff with extra paths
# also, skip 'warning: underquoted definition of' errors.
# if [ ! -e "aclocal.m4" ]; then 
	echo "-- aclocal.."
	if [ -e "$EXTRA_ACLOCAL_PATH" ]; then
		echo "-- - Searching extra path: $EXTRA_ACLOCAL_PATH"
		aclocal -I "$EXTRA_ACLOCAL_PATH" 2>&1 | grep -v 'warning: underquoted definition of' | grep -v 'Extending aclocal' | grep -v 'Extending-aclocal'
	else 
		echo "-- - Not searching extra path: $EXTRA_ACLOCAL_PATH"
		aclocal 2>&1 | grep -v 'warning: underquoted definition of' | grep -v 'Extending aclocal' | grep -v 'Extending-aclocal'
	fi
#else 
#	echo "-- skipping aclocal.."
#fi

echo "-- autoheader.. " && 		autoheader && \
echo "-- automake.." && 				automake --add-missing --copy && \
echo "-- autoconf.." && 				autoconf && \
echo "Done."
