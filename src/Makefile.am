bin_PROGRAMS = ninjas

AM_CXXFLAGS = -Wall

AM_LDFLAGS = -ggdb `allegro-config --libs` -lm
ninjas_LDFLAGS = @AM_LDFLAGS@

headers = \
force.h forceGravity.h forceInput.h gameOptions.h gameState.h \
globals.h input.h inputLiveHandler.h object.h objectFactory.h objectIDs.h \
objectPlayer.h objectRadiusBlock.h timer.h vec.h window.h \
gameBase.h objectBackground.h

ninjas_SOURCES = \
force.cpp forceGravity.cpp forceInput.cpp \
gameOptions.cpp gameState.cpp main.cpp gameBase.cpp \
input.cpp inputLiveHandler.cpp inputRecord.cpp \
object.cpp objectFactory.cpp objectPlayer.cpp objectRadiusBlock.cpp \
objectBackground.cpp \
timer.cpp vec.cpp window.cpp \
$(headers)

# EXTRA_DIST = dir1 dir2
# EXTRA_DIST = Doxygen
