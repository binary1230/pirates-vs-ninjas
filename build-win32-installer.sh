# a shortcut script to do all this for us.  At the end, we get a shiny 
# self-installing EXE file.
make clean && \
./autogen.sh && \
./cross-configure.sh --enable-release-build && \
./cross-make.sh && \
./cross-make.sh dist_win32_installer

echo -en "\n\n\nWin32 installer is ready.\n\n\n\n"
