#!/bin/sh

# EXPERT USERS ONLY
# see cross-configure.sh for info

TARGET=i386-mingw32msvc
BASE_PATH=/opt/xmingw
COMMON_PATH=$BASE_PATH/$TARGET

# ---------------------------------------------------------------
# DON'T MESS WITH ANYTHING BELOW HERE UNLESS YOU ARE REALLY SMART
# ---------------------------------------------------------------

export PATH="$WIN32_PATH:$BASE_PATH/bin:$PATH"

make $*
