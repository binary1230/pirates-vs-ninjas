#!/bin/sh

# EXPERT USERS ONLY
# see cross-configure.sh for info

BASE_PATH=/usr/i586-mingw32msvc

# ---------------------------------------------------------------
# DON'T MESS WITH ANYTHING BELOW HERE UNLESS YOU ARE REALLY SMART
# ---------------------------------------------------------------

export PATH="$BASE_PATH/bin:$PATH"

make $*
