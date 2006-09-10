bin_PROGRAMS = aitrainer

PRE_CXXCOMPILE = 	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
									$(AM_CPPFLAGS) $(CPPFLAGS) $(PRECOMPILED_CXXFLAGS) $(CXXFLAGS)

headers = \
aiTrainer.h stdafx.h precompiled.h

aitrainer_SOURCES = \
aiMain.cpp aiTrainer.cpp \
$(headers)

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
