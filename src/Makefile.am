SUBDIRS = tests

bin_PROGRAMS = ninjas

VPATH = external objects

# AM_CXXFLAGS = -Wall

# LDFLAGS = -ggdb -lm
# ninjas_LDFLAGS = @LDFLAGS@

# for profiling
# ninjas_LDFLAGS = -pg

ninjas_CXXFLAGS = -Wall @ninjas_CXXFLAGS@

headers = \
external/ezSockets.h external/alogg.h external/xmlParser.h \
external/StdString.h external/loadpng.h \
globals.h gameOptions.h gameState.h gameBase.h \
timer.h rect.h vector2D.h oggFile.h network.h basename.h \
window.h assetManager.h gameSound.h \
gameMode.h gameModes.h \
objectLayer.h objectFactory.h \
objects/object.h object/objectEnemy.h \
objects/objectPlayer.h objects/objectRadiusBlock.h \
objects/objectBackground.h objects/objectController.h \
objects/objectStatic.h objects/objectSpring.h \
objects/objectCollectable.h objects/objectFan.h objects/objectDoor.h \
input.h inputLiveHandler.h inputRecord.h inputPlayback.h \
force.h forceGravity.h forceInput.h forceFactory.h \
animation.h animations.h \
credits.h gameMenu.h physSimulation.h 

ninjas_SOURCES = \
external/ezSockets.cpp external/alogg.cpp external/xmlParser.cpp \
external/loadpng.cpp external/regpng.cpp external/savepng.cpp \
main.cpp gameOptions.cpp gameState.cpp gameBase.cpp \
timer.cpp rect.cpp vector2D.cpp oggFile.cpp network.cpp basename.cpp \
window.cpp assetManager.cpp gameSound.cpp \
gameMode.cpp gameModes.cpp \
objectLayer.cpp objectFactory.cpp \
objects/object.cpp objects/objectEnemy.h \
objects/objectPlayer.cpp objects/objectRadiusBlock.cpp \
objects/objectBackground.cpp objects/objectController.cpp \
objects/objectStatic.cpp objects/objectSpring.cpp \
objects/objectCollectable.cpp objects/objectFan.cpp objects/objectDoor.cpp \
input.cpp inputLiveHandler.cpp inputRecord.cpp inputPlayback.cpp \
force.cpp forceGravity.cpp forceInput.cpp forceFactory.cpp \
animation.cpp animations.cpp \
credits.cpp gameMenu.cpp physSimulation.cpp \
$(headers)

# EXTRA_DIST = dir1 dir2
EXTRA_DIST = Doxyfile Doxyfile.in
