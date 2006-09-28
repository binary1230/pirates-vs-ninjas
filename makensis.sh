#!/bin/bash

# Dom's custom script to generate the correct output for the NSIS 
# installer script

# NEAR:      Section "Game Files" SEC01

if [ "$#" != 1 ]; then
	echo "ERROR: Expected a path to distributable data dir."
	exit -1;
fi

DIST_DIR=$1

# almost done.
find "$DIST_DIR" -type d | grep -v '\.svn' | while read -r dir; do
	
	dir_text=`echo "$dir" | sed 's+/+\\\\+g'`
	echo '	SetOutPath "$INSTDIR\'$dir_text'"'

	find "$dir" -maxdepth 1 -not \( -type d -o -name .svn  \) -printf "%f\n" | while read -r file; do
		
		echo '	File "'$dir\\$file'"'

	done

done



