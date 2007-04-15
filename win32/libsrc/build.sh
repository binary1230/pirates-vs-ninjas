# build win32 libs, MASTER SCRIPT
# theoretically, all you should have to modify is this script.
#
# There are variables in cross-configure.sh and cross-make.sh that you 
# need to play with too.
#
# At the time I made this, this is what I had:
#
# alleggl-0.4.0
# allegro-4.2.1
# libogg-1.1.3
# libpng-1.2.16
# libvorbis-1.1.2
# lua-5.1.1
# zlib-1.2.3
# 
# Make sure to extract dx70_mgw.zip in the win32/ directory ahead of time.
# 
# You may also want to check that /usr/i586-mingw32msvc/bin/gcc (or whatever)
# is a symlink to i586-mingw32msvc-gcc (same for g++, ld, ar, and windres)
#
# You'll probably want to edit cross-configure.sh and cross-make.sh too.

export PATH=/usr/i586-mingw32msvc/bin:$PATH

if [ 0 == 1 ]; then

# -- ALLEGGL MODIFIED MAKEFILE PATCH --
echo '
Dom(s) patch to get allegro-gl cross compiling

--- make/makefile.mgw	2006-04-22 11:01:52.000000000 -0400
+++ make/cross-compile-ok-makefile	2007-03-08 22:47:55.000000000 -0500
@@ -3,3 +3,3 @@
 
-ifeq ($(findstring /sh.exe, $(SHELL)), /sh.exe)
+# ifeq ($(findstring /sh.exe, $(SHELL)), /sh.exe)
 UNIX_TOOLS=1
@@ -7,6 +7,6 @@
 PROG_INSTALLER = cp -f
-else
-DATA_INSTALLER = copy /y
-PROG_INSTALLER = copy /y
-endif
+#else
+#DATA_INSTALLER = copy /y
+#PROG_INSTALLER = copy /y
+#endif
 
@@ -121,3 +121,3 @@
 $(OBJ_DIR_U)/%.o: $(SRC_DIR_U)/%.c $(addprefix include/,$(C_DEPEND)) $(EXT_HEADERS_U)
-	$(CC) -c $< -o $@ $(FLAGS) $(INCLUDE_PATHS)
+	$(CC) -c $< -o $@ $(FLAGS) $(INCLUDE_PATHS) $(CFLAGS)
 
@@ -127,3 +127,3 @@
 $(EXAMPLE_DIR)/%.exe: $(EXAMPLE_DIR)/%.c $(HEADERS_U) $(LIB_PATH_U)
-	$(CC) $< -o $@ -mwindows $(FLAGS) $(INCLUDE_PATHS) $(LIB_PATH_U) $(COMPILER_LINK_LIB)
+	$(CC) $< -o $@ -mwindows $(FLAGS) $(INCLUDE_PATHS) $(LIB_PATH_U) $(COMPILER_LINK_LIB) $(CFLAGS) $(LDFLAGS)
 
' > alleggl-4.0.diff

# -- ALEGGRO COMMON XMAKE.SH --
echo '
XC_PATH=/usr/i586-mingw32msvc/bin
XPREFIX=i586-mingw32msvc-
INSTALL_BASE=/mnt/scratch/ninjas/win32

INCLUDE_DIR=$INSTALL_BASE/include
CROSSCOMPILE=1
MINGDIR=$INSTALL_BASE
DJDIR=$INSTALL_BASE
NATIVEPATH=$PATH
PATH=$XC_PATH:$NATIVEPATH
STATICLINK=1
UNIX_TOOLS=1

export CROSSCOMPILE MINGDIR DJDIR NATIVEPATH PATH XPREFIX STATICLINK INCLUDE_DIR

make $* LDFLAGS="-L$INSTALL_BASE/lib" CPPFLAGS="-I$INCLUDE_DIR" \
CFLAGS="-I$INSTALL_BASE/include"' \
> xmake.sh

# -- ALLEGRO GL --

cd alleggl*
patch -N -p0 < ../alleggl-4.0.diff
./fix.sh mingw32 && \
../xmake.sh
../xmake.sh install
cd ..

# -- ALLEGRO --

cd allegro-4*
./fix.sh mingw --quick && \
../xmake.sh && ./xmake.sh install
cd ..

# -- LUA --

cd lua*
make INSTALL_TOP=../../.. generic && \
make INSTALL_TOP=../../.. install
cd ..

# -- ZLIB --

cd zlib*
make clean
prefix=../../ ./configure
make
make install
cd ..

# --- LIBPNG --
cd libpng* && \
../cross-configure.sh --disable-shared --enable-static && \
../cross-make.sh && \
../cross-make.sh install
cd .. 

# --- LIBOGG --

cd libogg*
../cross-configure.sh --disable-shared --enable-static && \
../cross-make.sh && \
../cross-make.sh install
cd ..

# --- LIBVORBIS --

cd libvorbis*
../cross-configure.sh --disable-shared --enable-static && \
../cross-make.sh && \
../cross-make.sh install
cd ..


pushd /tmp
mkdir -p pthread-win32
cd pthread-win32
wget ftp://sources.redhat.com/pub/pthreads-win32/pthreads-w32-2-8-0-release.exe
unzip pthreads-w32-2-8-0-release.exe
popd
cp /tmp/pthread-win32/Pre-built.2/lib/libpthreadGCE2.a ../lib/libpthread.a
cp /tmp/pthread-win32/Pre-built.2/include/* ../include/

fi

cd CEGUI-0.5.0
../cross-configure.sh --disable-shared --enable-static && \
../cross-make.sh && \
../cross-make.sh install
cd ..


echo -en "\n\nDONE. Check make sure it's all good.\n\n"
