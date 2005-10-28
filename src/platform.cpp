// Deals with nasty platform specific stuff.  
// Tons of #ifdef fun

#include "platform.h"

#include <sys/types.h>
#include <sys/stat.h>

//! XXX STUPID HACK FOR NOW

//! Load a bitmap with the correct 'path'
//! Automagic function to wrap some platform crud
//! before it gets fixed in the real spriteLoader class
//! XXX MEMORY LEAK.  Yes I know.  It is stupid and temporary.
char* get_correct_path(const char* filename) {
	
	const int size=512;
	char *s = (char*)malloc(size);
	
	const char* path = get_current_working_dir();
	// const char* path = "/path/to/whatever.app/Resources/"	

	strncpy(s, path, size-1);
	strncat(s, "Resources/", size-1);
	strncat(s, filename, size-1);

	// couldn't find it.. try..
	struct stat sb;
	if (stat(s, &sb) == -1) {
		strncpy(s, filename, size-1);
	}

	return s;
}

// ONLY needed for MacOSX bundles
#ifdef PLATFORM_DARWIN 

#include <CoreServices/CoreServices.h>
	//! Returns something like '/Applications/Ninjas.app/'
	const char* get_current_working_dir() {
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef url = CFBundleCopyBundleURL(mainBundle);
		CFStringRef cfStr = CFURLCopyPath(url);
		return CFStringGetCStringPtr(cfStr, CFStringGetSystemEncoding());
	}
#else
	char* get_current_working_dir() {
		return "";
	}
#endif
