bin_PROGRAMS = ninjas

AM_CXXFLAGS = -Wall

AM_LDFLAGS = -ggdb -lm
ninjas_LDFLAGS = @AM_LDFLAGS@

headers = \
globals.h timer.h \
gameOptions.h gameState.h gameBase.h window.h \
input.h inputLiveHandler.h inputRecord.h inputPlayback.h \
object.h objectFactory.h objectIDs.h \
objectPlayer.h objectRadiusBlock.h objectBackground.h \
vec.h force.h forceGravity.h forceInput.h

ninjas_SOURCES = \
force.cpp forceGravity.cpp forceInput.cpp \
gameOptions.cpp gameState.cpp main.cpp gameBase.cpp \
input.cpp inputLiveHandler.cpp inputRecord.cpp inputPlayback.cpp \
object.cpp objectFactory.cpp objectPlayer.cpp objectRadiusBlock.cpp \
objectBackground.cpp \
timer.cpp vec.cpp window.cpp \
$(headers)

# EXTRA_DIST = dir1 dir2
# EXTRA_DIST = Doxygen
