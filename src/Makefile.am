bin_PROGRAMS = ninjas

CXXFLAGS = "-Wall"

LDFLAGS = -ggdb `allegro-config --libs` -lm
ninjas_LDFLAGS = @LDFLAGS@

headers = \
force.h forceGravity.h forceInput.h gameOptions.h gameState.h \
globals.h input.h inputLiveHandler.h object.h objectFactory.h objectIDs.h \
objectPlayer.h objectRadiusBlock.h timer.h vec.h window.h \
gameBase.h

ninjas_SOURCES = \
force.cpp forceGravity.cpp forceInput.cpp \
gameOptions.cpp gameState.cpp main.cpp gameBase.cpp \
input.cpp inputLiveHandler.cpp \
object.cpp objectFactory.cpp objectPlayer.cpp objectRadiusBlock.cpp \
timer.cpp vec.cpp window.cpp \
$(headers)

# EXTRA_DIST = dir1 dir2
# EXTRA_DIST = Doxygen
