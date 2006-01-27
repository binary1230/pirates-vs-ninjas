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

# --------------------------------------------------------------
# set the following variables (you don't _have_ to set them all,
# but weird things can happen if you don't)

# should contain cross compiled 'allegro-config'
WIN32_PATH=/usr/i586-mingw32msvc/bin

# should contain cross compiled liballeg.a
WIN32_LIBS=/usr/i586-mingw32msvc/lib

# should contain cross compiled allegro.h
WIN32_INCLUDES=/usr/i586-mings32msvc/include

# --------------------------------------------------------------
# DON'T MESS WITH ANYTHING BELOW HERE UNLESS YOU ARE SUPER-SMART
# --------------------------------------------------------------

PATH="$WIN32_PATH:$PATH"
export PATH

./configure --target=i586-mingw32msvc --host=i586-mingw32msvc --build=linux-i386 --with-extra-includes=$WIN32_INCLUDES --with-extra-libs=$WIN32_LIBS $*
