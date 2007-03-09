#!/bin/sh

# EXPERT USERS ONLY
# Script to cross compile ninja-engine from a linux host
# to a windows .exe file (mingw32).

# use this script like you would normally use ./configure
# 
# for example, if you normally do './configure --prefix=/whatever'
# you would do './cross-configure --prefix=/whatever'

# REQUIRES:
#  mingw32 to be installed (must be called i586-mingw32-g++)
#  allegro to be BUILT as a cross compiled library.

# if stuff still doesn't work here, you can edit the configure.in
# look at the secion marked 'XXXX CROSS COMPILE'
# ALSO, make sure to check the BUILD file and the WIKI for info
# cross compiling support is STILL very flaky.  Try messing with stuff

# --------------------------------------------------------------
# set the following variables (you don't _have_ to set them all,
# but weird things can happen if you don't)

TARGET=i586-mingw32msvc
BASE_PATH=/usr/
COMMON_PATH=$BASE_PATH/$TARGET/bin

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# the following _should_ be OK, but adjust if you need to
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# should contain cross compiled 'allegro-config'
WIN32_PATH=`pwd`/

# should contain cross compiled 
WIN32_LIBS=$WIN32_PATH/win32/lib

# should contain cross compiled allegro.h
WIN32_INCLUDES=$WIN32_PATH/win32/include

OGG_PREFIX=$WIN32_PATH/win32
OGG_INCLUDES=$WIN32_PATH/win32/include
VORBIS_PREFIX=$WIN32_PATH/win32
VORBIS_INCLUDES=$WIN32_PATH/win32/include

# ---------------------------------------------------------------
# DON'T MESS WITH ANYTHING BELOW HERE UNLESS YOU ARE REALLY SMART
# ---------------------------------------------------------------

export PATH="$BASE_PATH/bin:$PATH"

./configure --target=$TARGET --host=$TARGET --build=i386-linux \
--with-extra-includes=$WIN32_INCLUDES --with-extra-libs=$WIN32_LIBS \
--with-ogg=$OGG_PREFIX --with-ogg-includes=$OGG_INCLUDES \
--with-vorbis=$VORBIS_PREFIX  --with-vorbis-includes=$VORBIS_INCLUDES \
 $*
