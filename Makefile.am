if DEBUG
SUBDIRS = src # ai
else
SUBDIRS = src
endif

DATA_DIR = data
EXTRA_DIST = docs $(DATA_DIR) installer.nsi 
	
DIST_DIR = dist/win32/$(PACKAGE_NAME)-$(VERSION)

# create the MAC OSX Bundle
dist_macosx: $(DISTFILES)
	rm -rf dist/darwin/*
	mkdir -p dist/darwin/ninjas.app/Contents/MacOS
	mkdir dist/darwin/ninjas.app/PkgInfo
	mkdir dist/darwin/ninjas.app/Resources
	cp src/ninjas dist/darwin/ninjas.app/Contents/MacOS
	cp -r $(DATA_DIR) dist/darwin/ninjas.app/Resources
	hdiutil create -fs HFS+ -volname PiratesVersusNinjas-$(VERSION) -srcfolder dist/darwin/ PiratesVersusNinjas-$(VERSION)


# common win32 packaging stuff
dist_win32_common: $(DISTFILES)
	rm -rf dist/win32/$(PACKAGE_NAME)-*
	mkdir -p $(DIST_DIR)/data
	cp src/ninjas.exe $(DIST_DIR)
	strip $(DIST_DIR)/ninjas.exe
	cp LICENSE README $(DIST_DIR)
	cp dist/win32/common/* $(DIST_DIR)
	cp -r $(DATA_DIR) $(DIST_DIR)


# create the windows NSIS self-installer
# TODO: Remove wine hardcoded NSIS path
dist_win32_installer: dist_win32_common 
	./makensis.sh dist/win32/ninjas-engine-$(VERSION)
#	makensis installer.nsi
	wine C:/Program\ Files/NSIS/makensis.exe installer.nsi
	

# create the windows .zip file
dist_win32_zip: dist_win32_common 
	cd dist/win32/ && zip -r ninjas-engine-$(VERSION).zip ninjas-engine-$(VERSION) -x \*.svn\*
	
	@echo -en "\n\nWindows version packaged: $(DIST_DIR).zip\n\n"


doc: src/Doxyfile
	cd src && doxygen

test: $(DISTFILES)
	src/ninjas
