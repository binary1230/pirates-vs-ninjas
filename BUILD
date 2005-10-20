Currently you can build for win32 or linux

Allegro dev packages are required

--------------------
LINUX BUILD (normal)
--------------------
untar everything somewhere (say ~/ninjas)

 cd ~/ninjas
 make
 ./ninjas

---------------------------------------------------
WINDOWS BUILD (cross-compile)
do this under linux to produce a windows .exe file
setting up allegro is tricky this way.
---------------------------------------------------
untar everything somewhere (say ~/ninjas)

 cd ~/ninjas/src
 [edit Makefile.mgw for your paths]
 make -f Makefile.mgw
 cd ..
 [copy data/, ninjas.exe, and alleg40.dll for distribution]

----------------------------------------------
SUBVERSION (CVS) Notes
----------------------------------------------
[is there a better way than this?]

If you want to include the subversion revision number
in the actual game executable, there is a script which
can extract it and write it out into "auto-config.h"

THIS IS HIGHLY RECOMMENDED FOR BETA TESTING USE
This way, we can identify which subversion tree is causing
the problems that beta testers tell us about

do the following: (only works in linux for now):
 1) commit any changes ('svn commit -m "comments about change"')
 2) 'svn up' to update your working copy to the correct revision number
 3) './regen-cvs-tags' to rewrite auto-config.h
 4) build normally

The revision number is output to the console on game startup and is also
the title of the window.

--------------------------------------------------------
TO BUILD THE DOCUMENTATION
--------------------------------------------------------
I use doxygen to generate API info from the source.

To build these docs type 'make doc' from the src/ dir.

The results will be put in docs/html/
That dir is NOT included in the subversion tree.
