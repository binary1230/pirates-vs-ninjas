#include "resourceLoader.h"

#ifdef PLATFORM_DARWIN
#include <CoreServices/CoreServices.h>
#endif 

int ResourceLoader::Init(GameState* _game_state) {
	SetGameState(_game_state);
	ResetPaths();
	return true;
}

void ResourceLoader::Shutdown() {
	paths.clear();
}

// XXXX using '..' doesn't work yet, (allegro's load_bitmap()'s fault)
void ResourceLoader::AppendToSearchPath(const char* path) {
	if (path && path[0])
		paths.push_back(path);
}

void ResourceLoader::ResetPaths() {
	paths.clear();
	
	// Only for MacOSX paths
	CString MacOSXWorkingDir = GetMacOSXCurrentWorkingDir();
	if (MacOSXWorkingDir != "") {
		paths.push_back(MacOSXWorkingDir);
	}
	
	// For everyone else..
	paths.push_back("./");
}

//! Returns either the full path to a real file,
//! or an empty CString
CString ResourceLoader::GetPathOf(const char* filename) const {
	
	const CString seperator = "/";
	CString fullpath;
				
	for (uint i = 0; i < paths.size(); i++) {
		fullpath = paths[i] + seperator + filename;
		if (FileExists(fullpath))
			return fullpath;
	}
		
	return "";
}

//! Returns true if a file exists, false otherwise
bool ResourceLoader::FileExists(const char* file) const {
	struct stat sb;
	
	if (stat(file, &sb) == -1)
		return false;
		
	return true;
}

//! Opens a bitmap, utilizes the search paths
BITMAP* ResourceLoader::OpenBitmap(const char* filename, PALETTE* pal) const {
	BITMAP* bmp = NULL;
	CString file = GetPathOf(filename);
	
	if (file.length() != 0)
		bmp = load_bitmap(file, *pal);
	
	return bmp;
}

ResourceLoader::ResourceLoader() {
	ResetPaths();
}

ResourceLoader::~ResourceLoader() {
}

//  -------------------------------------------------------------------------
//  MacOSX Hack-ish thing.
//  -------------------------------------------------------------------------
//! MacOSX bundles (the way we distribute the game) does not allow
//! us to use relative paths because the working directory on MacOSX
//! is not set to the directory where the executable file lives.
//! 
//! What we need to do on Mac is call some Mac-specific code to 
//! find out where the executable lives, and then open our game files
//! relative to that path.
//!
//! On all other platforms, however, we can just use relative paths.
//! This function will attempt to use the MacOSX way first, and if
//! that fails, will just run the relative path way as fallback.

//! Returns something like "/Applications/Ninjas.app/Resources" on Mac, 
//! if not on Mac, it just returns ""
CString ResourceLoader::GetMacOSXCurrentWorkingDir() const {
#ifdef PLATFORM_DARWIN 
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyBundleURL(mainBundle);
	CFStringRef cfStr = CFURLCopyPath(url);
	CString path = CFStringGetCStringPtr(cfStr, CFStringGetSystemEncoding());
	return CString(path + "Resources/");
#else
	return "";
#endif // PLATFORM_DARWIN
}

