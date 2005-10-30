SUBDIRS = src

EXTRA_DATA = data
EXTRA_DIST = docs $(EXTRA_DATA)

# create the MAC OSX Bundle
dist_macosx: $(DISTFILES)
	rm -rf dist/darwin/*
	mkdir -p dist/darwin/ninjas.app/Contents/MacOS
	mkdir dist/darwin/ninjas.app/PkgInfo
	mkdir dist/darwin/ninjas.app/Resources
	cp src/ninjas dist/darwin/ninjas.app/Contents/MacOS
	cp -r $(EXTRA_DATA) dist/darwin/ninjas.app/Resources

# create the windows .zip file
dist_win32: $(DISTFILES) 
	rm -rf dist/win32/*
	mkdir -p dist/win32/$(VERSION)/data
	cp src/ninjas.exe dist/win32/$(VERSION)/
	cp -r $(EXTRA_DATA) dist/win32/data
	cp LICENSE README dist/win32/data
	zip -r dist/win32/$(VERSION).zip dist/win32/$(VERSION)
	echo "Windows version packaged: dist/win32/$(VERSION).zip"

doc: src/Doxyfile
	cd src && doxygen

test: $(DISTFILES)
	src/ninjas
