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

dist_win32: $(DISTFILES)
	echo "yes."
