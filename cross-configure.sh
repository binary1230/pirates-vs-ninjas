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
BASE_PATH=/opt/i586-mingw32msvc
COMMON_PATH=$BASE_PATH/$TARGET

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# the following _should_ be OK, but adjust if you need to
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# should contain cross compiled 'allegro-config'
WIN32_PATH=$COMMON_PATH/bin

# should contain cross compiled liballeg.a
WIN32_LIBS=$COMMON_PATH/lib

# should contain cross compiled allegro.h
WIN32_INCLUDES=$COMMON_PATH/include

OGG_PREFIX=$COMMON_PATH
OGG_INCLUDES=$WIN32_INCLUDES
VORBIS_PREFIX=$COMMON_PATH
VORBIS_INCLUDES=$WIN32_INCLUDES

# ---------------------------------------------------------------
# DON'T MESS WITH ANYTHING BELOW HERE UNLESS YOU ARE REALLY SMART
# ---------------------------------------------------------------

export PATH="$WIN32_PATH:$BASE_PATH/bin:$PATH"

./configure --target=$TARGET --host=$TARGET --build=linux-i386 \
--with-extra-includes=$WIN32_INCLUDES --with-extra-libs=$WIN32_LIBS \
--with-ogg=$OGG_PREFIX --with-ogg-includes=$OGG_INCLUDES \
--with-vorbis=$VORBIS_PREFIX  --with-vorbis-includes=$VORBIS_INCLUDES \
 $*
