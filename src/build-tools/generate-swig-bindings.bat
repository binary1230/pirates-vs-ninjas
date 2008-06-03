@echo off
REM    NOTE: If you modify these options, change them in Makefile.am
REM    or else you will break the linux build.  Ask Dom.
REM
REM    TODO: Read options from a file or something.

cd build-tools\swig
echo ------------------------------------------------------------
echo Starting SWIG...
swig.exe -o ..\..\swig\engine_wrap.cpp -c++ -lua ..\..\swig\engine.i
echo Done generating bindings.
echo ------------------------------------------------------------
cd ..\..
