# makefile for linux/*nix
include Makefile.common

GXX = g++
GCC = gcc
CFLAGS = -c -Wall -g
LDFLAGS = -ggdb `allegro-config --libs` -lm
			
TARGET = ninjas

OBJS = $(SRCS:.cpp=.o)

.PHONY: depend clean

all: target

target: $(OBJS) 
	$(GXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
	mv $(TARGET) ..
	
.cpp.o:
	$(GXX) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o core $(TARGET) ../$(TARGET)

doc: 
	doxygen
	
test: target
	cd .. && ./$(TARGET)

depend: $(SRCS)
	# OLD WAY
	#	makedepend $(INCLUDES) $^
	
	#	NEW WAY
	gccmakedep -- $(CFLAGS) -- $(SRCS)

# DO NOT DELETE
main.o: main.cpp /usr/include/allegro.h /usr/include/allegro/base.h \
  /usr/include/errno.h /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h /usr/include/stdio.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/include/bits/sys_errlist.h gameState.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/vector \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/functexcept.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_algobase.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++config.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/os_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstring \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstddef \
  /usr/include/string.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/climits \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdlib \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/iosfwd \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++locale.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdio \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/clocale \
  /usr/include/locale.h /usr/include/bits/locale.h \
  /usr/include/langinfo.h /usr/include/nl_types.h /usr/include/iconv.h \
  /usr/include/libintl.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++io.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr-default.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h \
  /usr/include/bits/initspin.h /usr/include/bits/sigthread.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cctype \
  /usr/include/ctype.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stringfwd.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/postypes.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cwchar \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ctime \
  /usr/include/stdint.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_pair.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/cpp_type_traits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_types.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_funcs.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/concept_check.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/debug/debug.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cassert \
  /usr/include/assert.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ext/mt_allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/new \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/atomicity.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/atomic_word.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_construct.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_uninitialized.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_vector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_bvector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/vector.tcc \
  gameOptions.h globals.h auto-config.h window.h timer.h input.h \
  inputLiveHandler.h object.h objectFactory.h objectIDs.h objectPlayer.h \
  /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
  /usr/include/bits/inf.h /usr/include/bits/nan.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h vec.h force.h \
  forceGravity.h forceInput.h objectRadiusBlock.h
gameOptions.o: gameOptions.cpp gameOptions.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h globals.h \
  auto-config.h
gameState.o: gameState.cpp gameState.h /usr/include/allegro.h \
  /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h /usr/include/stdio.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/include/bits/sys_errlist.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/vector \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/functexcept.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_algobase.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++config.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/os_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstring \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstddef \
  /usr/include/string.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/climits \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdlib \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/iosfwd \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++locale.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdio \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/clocale \
  /usr/include/locale.h /usr/include/bits/locale.h \
  /usr/include/langinfo.h /usr/include/nl_types.h /usr/include/iconv.h \
  /usr/include/libintl.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++io.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr-default.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h \
  /usr/include/bits/initspin.h /usr/include/bits/sigthread.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cctype \
  /usr/include/ctype.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stringfwd.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/postypes.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cwchar \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ctime \
  /usr/include/stdint.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_pair.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/cpp_type_traits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_types.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_funcs.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/concept_check.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/debug/debug.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cassert \
  /usr/include/assert.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ext/mt_allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/new \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/atomicity.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/atomic_word.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_construct.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_uninitialized.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_vector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_bvector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/vector.tcc \
  gameOptions.h globals.h auto-config.h window.h timer.h input.h \
  inputLiveHandler.h object.h objectFactory.h objectIDs.h objectPlayer.h \
  /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
  /usr/include/bits/inf.h /usr/include/bits/nan.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h vec.h force.h \
  forceGravity.h forceInput.h objectRadiusBlock.h
window.o: window.cpp window.h /usr/include/allegro.h \
  /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h /usr/include/stdio.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/include/bits/sys_errlist.h globals.h auto-config.h
timer.o: timer.cpp timer.h /usr/include/allegro.h \
  /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h
input.o: input.cpp input.h /usr/include/allegro.h \
  /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h
inputLiveHandler.o: inputLiveHandler.cpp inputLiveHandler.h \
  /usr/include/allegro.h /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h input.h
objectFactory.o: objectFactory.cpp objectFactory.h /usr/include/allegro.h \
  /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h /usr/include/stdio.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/include/bits/sys_errlist.h globals.h auto-config.h object.h \
  objectIDs.h objectPlayer.h /usr/include/math.h \
  /usr/include/bits/huge_val.h /usr/include/bits/huge_valf.h \
  /usr/include/bits/huge_vall.h /usr/include/bits/inf.h \
  /usr/include/bits/nan.h /usr/include/bits/mathdef.h \
  /usr/include/bits/mathcalls.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/vector \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/functexcept.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_algobase.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++config.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/os_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstring \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstddef \
  /usr/include/string.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/climits \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdlib \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/iosfwd \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++locale.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdio \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/clocale \
  /usr/include/locale.h /usr/include/bits/locale.h \
  /usr/include/langinfo.h /usr/include/nl_types.h /usr/include/iconv.h \
  /usr/include/libintl.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++io.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr-default.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h \
  /usr/include/bits/initspin.h /usr/include/bits/sigthread.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cctype \
  /usr/include/ctype.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stringfwd.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/postypes.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cwchar \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ctime \
  /usr/include/stdint.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_pair.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/cpp_type_traits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_types.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_funcs.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/concept_check.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/debug/debug.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cassert \
  /usr/include/assert.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ext/mt_allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/new \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/atomicity.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/atomic_word.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_construct.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_uninitialized.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_vector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_bvector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/vector.tcc \
  input.h inputLiveHandler.h vec.h force.h forceGravity.h forceInput.h \
  objectRadiusBlock.h
object.o: object.cpp object.h /usr/include/allegro.h \
  /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h /usr/include/stdio.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/include/bits/sys_errlist.h globals.h auto-config.h
objectRadiusBlock.o: objectRadiusBlock.cpp objectRadiusBlock.h \
  /usr/include/math.h /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/huge_val.h \
  /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
  /usr/include/bits/inf.h /usr/include/bits/nan.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  /usr/include/stdio.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h object.h \
  /usr/include/allegro.h /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/bits/errno.h /usr/include/linux/errno.h \
  /usr/include/asm/errno.h /usr/include/asm-generic/errno.h \
  /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h globals.h auto-config.h
objectPlayer.o: objectPlayer.cpp objectPlayer.h /usr/include/allegro.h \
  /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h /usr/include/math.h \
  /usr/include/bits/huge_val.h /usr/include/bits/huge_valf.h \
  /usr/include/bits/huge_vall.h /usr/include/bits/inf.h \
  /usr/include/bits/nan.h /usr/include/bits/mathdef.h \
  /usr/include/bits/mathcalls.h /usr/include/stdio.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h /usr/include/bits/sys_errlist.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/vector \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/functexcept.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_algobase.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++config.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/os_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstring \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstddef \
  /usr/include/string.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/climits \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdlib \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/iosfwd \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++locale.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdio \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/clocale \
  /usr/include/locale.h /usr/include/bits/locale.h \
  /usr/include/langinfo.h /usr/include/nl_types.h /usr/include/iconv.h \
  /usr/include/libintl.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++io.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr-default.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h \
  /usr/include/bits/initspin.h /usr/include/bits/sigthread.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cctype \
  /usr/include/ctype.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stringfwd.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/postypes.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cwchar \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ctime \
  /usr/include/stdint.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_pair.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/cpp_type_traits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_types.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_funcs.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/concept_check.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/debug/debug.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cassert \
  /usr/include/assert.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ext/mt_allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/new \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/atomicity.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/atomic_word.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_construct.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_uninitialized.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_vector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_bvector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/vector.tcc \
  globals.h auto-config.h input.h inputLiveHandler.h object.h vec.h \
  force.h forceGravity.h forceInput.h
vec.o: vec.cpp vec.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
  /usr/include/bits/inf.h /usr/include/bits/nan.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/vector \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/functexcept.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_algobase.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++config.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/os_defines.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstring \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstddef \
  /usr/include/string.h /usr/include/xlocale.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/climits \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdlib \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/types.h /usr/include/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/bits/time.h \
  /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/alloca.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/iosfwd \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++locale.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cstdio \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/clocale \
  /usr/include/locale.h /usr/include/bits/locale.h \
  /usr/include/langinfo.h /usr/include/nl_types.h /usr/include/iconv.h \
  /usr/include/libintl.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++io.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/gthr-default.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/signal.h \
  /usr/include/bits/initspin.h /usr/include/bits/sigthread.h \
  /usr/include/unistd.h /usr/include/bits/posix_opt.h \
  /usr/include/bits/environments.h /usr/include/bits/confname.h \
  /usr/include/getopt.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cctype \
  /usr/include/ctype.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stringfwd.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/postypes.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cwchar \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ctime \
  /usr/include/stdint.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_pair.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/cpp_type_traits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_types.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator_base_funcs.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/concept_check.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_iterator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/debug/debug.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/cassert \
  /usr/include/assert.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/c++allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/ext/mt_allocator.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/new \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/exception \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/atomicity.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/i486-linux-gnu/bits/atomic_word.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_construct.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_uninitialized.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_vector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/stl_bvector.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/../../../../include/c++/4.0.2/bits/vector.tcc \
  force.h
force.o: force.cpp force.h
forceGravity.o: forceGravity.cpp forceGravity.h force.h
forceInput.o: forceInput.cpp forceInput.h /usr/include/allegro.h \
  /usr/include/allegro/base.h /usr/include/errno.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/limits.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /usr/include/bits/stdio_lim.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stdarg.h \
  /cdrom/kfs/usr/bin/../lib/gcc/i486-linux-gnu/4.0.2/include/stddef.h \
  /usr/include/stdlib.h /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h \
  /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h \
  /usr/include/alloca.h /usr/include/allegro/internal/alconfig.h \
  /usr/include/allegro/platform/alplatf.h \
  /usr/include/allegro/platform/alucfg.h /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h /usr/include/sys/stat.h \
  /usr/include/bits/stat.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  /usr/include/allegro/platform/alunixac.h /usr/include/allegro/system.h \
  /usr/include/allegro/base.h /usr/include/allegro/inline/system.inl \
  /usr/include/allegro/debug.h /usr/include/allegro/debug.h \
  /usr/include/allegro/unicode.h /usr/include/allegro/mouse.h \
  /usr/include/allegro/timer.h /usr/include/allegro/keyboard.h \
  /usr/include/allegro/joystick.h /usr/include/allegro/palette.h \
  /usr/include/allegro/gfx.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/inline/asm.inl \
  /usr/include/allegro/platform/al386gcc.h /usr/include/allegro/color.h \
  /usr/include/allegro/palette.h /usr/include/allegro/inline/color.inl \
  /usr/include/allegro/draw.h /usr/include/allegro/gfx.h \
  /usr/include/allegro/inline/draw.inl \
  /usr/include/allegro/inline/gfx.inl /usr/include/allegro/rle.h \
  /usr/include/allegro/inline/rle.inl /usr/include/allegro/compiled.h \
  /usr/include/allegro/text.h /usr/include/allegro/fli.h \
  /usr/include/allegro/config.h /usr/include/allegro/gui.h \
  /usr/include/allegro/sound.h /usr/include/allegro/digi.h \
  /usr/include/allegro/stream.h /usr/include/allegro/midi.h \
  /usr/include/allegro/file.h /usr/include/allegro/inline/file.inl \
  /usr/include/allegro/datafile.h /usr/include/allegro/fixed.h \
  /usr/include/allegro/fmaths.h /usr/include/allegro/inline/fmaths.inl \
  /usr/include/allegro/matrix.h /usr/include/allegro/fmaths.h \
  /usr/include/allegro/inline/matrix.inl /usr/include/allegro/quat.h \
  /usr/include/allegro/3d.h /usr/include/allegro/3dmaths.h \
  /usr/include/allegro/inline/3dmaths.inl /usr/include/allegro/alcompat.h \
  /usr/include/allegro/fix.h /usr/include/allegro/inline/fix.inl \
  /usr/include/allegro/platform/alunix.h force.h
