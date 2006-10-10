SUBDIRS = tests
bin_PROGRAMS = ninjas 

VPATH = external objects

PRE_CXXCOMPILE = 	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
									$(AM_CPPFLAGS) $(CPPFLAGS) $(PRECOMPILED_CXXFLAGS) $(CXXFLAGS)

# AM_CXXFLAGS = -Wall

# LDFLAGS = -ggdb -lm
# ninjas_LDFLAGS = @LDFLAGS@

# for profiling
# ninjas_LDFLAGS = -pg

# ninjas_CXXFLAGS = @ninjas_CXXFLAGS@ -include precompiled.h

# for alfont, not used right now
# external/alfont.h external/alfontdll.h 
# external/alfont.cpp 

headers = \
external/ezSockets.h external/alogg.h external/aloggdll.h \
external/xmlParser.h external/StdString.h external/loadpng.h \
globals.h gameOptions.h gameState.h \
timer.h rect.h vector2D.h oggFile.h network.h basename.h \
window.h assetManager.h gameSound.h \
gameMode.h gameModes.h \
objectLayer.h objectFactory.h objectIDs.h \
objects/object.h objects/objectEnemy.h \
objects/objectPlayer.h objects/objectBounce.h \
objects/objectBackground.h objects/objectController.h \
objects/objectStatic.h objects/objectSpring.h objects/objectTxtOverlay.h \
objects/objectCollectable.h objects/objectFan.h objects/objectDoor.h \
input.h \
force.h forceGravity.h forceInput.h forceFactory.h \
animation.h animations.h singleton.h \
credits.h gameMenu.h physSimulation.h sprite.h precompiled.h stdafx.h 

ninjas_SOURCES = \
external/ezSockets.cpp external/alogg.cpp external/xmlParser.cpp \
external/loadpng.cpp external/regpng.cpp external/savepng.cpp \
main.cpp gameOptions.cpp gameState.cpp globals.cpp \
timer.cpp rect.cpp vector2D.cpp oggFile.cpp network.cpp basename.cpp \
window.cpp assetManager.cpp gameSound.cpp \
gameMode.cpp gameModes.cpp \
objectLayer.cpp objectFactory.cpp \
objects/object.cpp objects/objectEnemy.h \
objects/objectPlayer.cpp objects/objectBounce.cpp \
objects/objectBackground.cpp objects/objectController.cpp \
objects/objectStatic.cpp objects/objectSpring.cpp objects/objectTxtOverlay.cpp \
objects/objectCollectable.cpp objects/objectFan.cpp objects/objectDoor.cpp \
input.cpp \
force.cpp forceGravity.cpp forceInput.cpp forceFactory.cpp \
animation.cpp animations.cpp \
credits.cpp gameMenu.cpp physSimulation.cpp \
$(headers)

# EXTRA_DIST = dir1 dir2
EXTRA_DIST = Doxyfile Doxyfile.in

# if we're using precompiled headers...
if PRECOMPILE_ENABLE

BUILT_SOURCES = precompiled.h.gch
CLEANFILES = 		precompiled.h.gch

include ./$(DEPDIR)/precompiled.h.Po

$(DEPDIR)/precompiled.h.Po:
	if test ! -d "$(DEPDIR)"; then mkdir $(DEPDIR); fi
	touch $(DEPDIR)/precompiled.h.Po
	
precompiled.h.gch: precompiled.h
	if $(PRE_CXXCOMPILE) -x c++-header -MT $@ -MD -MP -MF "$(DEPDIR)/precompiled.h.Tpo" \
	-c -o $@ $<; then mv -f "$(DEPDIR)/precompiled.h.Tpo" "$(DEPDIR)/precompiled.h.Po"; \
	else rm -f "$(DEPDIR)/precompiled.h.Tpo"; exit 1; fi;

endif
