SUBDIRS = libs/alogg src

DATA_DIR = data
EXTRA_DIST = docs $(DATA_DIR) 

dist_common:
	make -C libs/alogg clean

# create the MAC OSX Bundle
dist_macosx: dist_common $(DISTFILES)
	rm -rf dist/darwin/*
	mkdir -p dist/darwin/ninjas.app/Contents/MacOS
	mkdir dist/darwin/ninjas.app/PkgInfo
	mkdir dist/darwin/ninjas.app/Resources
	cp src/ninjas dist/darwin/ninjas.app/Contents/MacOS
	cp -r $(DATA_DIR) dist/darwin/ninjas.app/Resources
	hdiutil create -fs HFS+ -volname PiratesVersusNinjas-$(VERSION) -srcfolder dist/darwin/ PiratesVersusNinjas-$(VERSION)

# create the windows .zip file
dist_win32: dist_common $(DISTFILES) 
	rm -rf dist/win32/ninjas-engine-*
	mkdir -p dist/win32/ninjas-engine-$(VERSION)/data
	
	cp src/ninjas.exe dist/win32/ninjas-engine-$(VERSION)
	strip dist/win32/ninjas-engine-$(VERSION)/ninjas.exe
	cp LICENSE README dist/win32/ninjas-engine-$(VERSION)
	cp dist/win32/common/* dist/win32/ninjas-engine-$(VERSION)
	cp -r $(DATA_DIR) dist/win32/ninjas-engine-$(VERSION)
	cd dist/win32/ && zip -r ninjas-engine-$(VERSION).zip ninjas-engine-$(VERSION) -x \*.svn\*
	
	@echo -en "\n\nWindows version packaged: dist/win32/ninjas-engine-$(VERSION).zip\n\n"

doc: src/Doxyfile
	cd src && doxygen

test: $(DISTFILES)
	src/ninjas
