SUBDIRS = tests

bin_PROGRAMS = ninjas

# AM_CXXFLAGS = -Wall

# LDFLAGS = -ggdb -lm
# ninjas_LDFLAGS = @LDFLAGS@

# for profiling
# ninjas_LDFLAGS = -pg

ninjas_CXXFLAGS = -Wall

headers = \
globals.h timer.h \
gameOptions.h gameState.h gameBase.h window.h \
gameMode.h physSimulation.h \
input.h inputLiveHandler.h inputRecord.h inputPlayback.h gameSound.h \
object.h objectFactory.h objectIDs.h \
objectPlayer.h objectRadiusBlock.h objectBackground.h objectController.h \
objectStatic.h objectSpring.h objectCollectable.h\
rect.h vector2D.h force.h forceFactory.h \
forceGravity.h forceInput.h \
basename.h assetManager.h \
animation.h animations.h sprite.h \
StdString.h xmlParser.h objectLayer.h 

ninjas_SOURCES = \
force.cpp forceGravity.cpp forceInput.cpp forceFactory.cpp \
gameOptions.cpp gameState.cpp main.cpp gameBase.cpp \
gameMode.cpp physSimulation.cpp objectLayer.cpp \
gameSound.cpp input.cpp inputLiveHandler.cpp inputRecord.cpp inputPlayback.cpp \
object.cpp objectFactory.cpp objectPlayer.cpp objectRadiusBlock.cpp \
objectBackground.cpp objectController.cpp objectStatic.cpp \
objectSpring.cpp objectCollectable.cpp\
timer.cpp rect.cpp vector2D.cpp window.cpp \
basename.cpp assetManager.cpp \
animation.cpp animations.cpp \
xmlParser.cpp \
$(headers)

# EXTRA_DIST = dir1 dir2
EXTRA_DIST = Doxyfile Doxyfile.in
