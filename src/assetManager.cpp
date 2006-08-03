#include "assetManager.h"

#ifdef PLATFORM_DARWIN
#include <CoreServices/CoreServices.h>
#endif 

int AssetManager::Init(GameState* _game_state) {
	SetGameState(_game_state);
	ResetPaths();
	bitmaps.clear();
	samples.clear();
	return true;
}

void AssetManager::Free() {
	FreeBitmaps();
	FreeSamples();
}

// XXX should make these templated...
void AssetManager::FreeSamples() {
	SampleListIter i;
	for (i = samples.begin(); i != samples.end(); i++) {
		if (i->second)
			destroy_sample(i->second);
	}
	samples.clear();
}

void AssetManager::FreeBitmaps() {
	BitmapListIter i;
	for (i = bitmaps.begin(); i != bitmaps.end(); i++) {
		if (i->second)
			destroy_bitmap(i->second);
	}
	bitmaps.clear();
}

/*void AssetManager::FreeSongs() {
	SongListIter i;
	for (i = songs.begin(); i != songs.end(); i++) {
		if (i->second)
			(i->second);
	}
	songs.clear();
}*/



void AssetManager::Shutdown() {	
	Free();
	paths.clear();
}

// XXXX using '..' doesn't work yet, (allegro's load_bitmap()'s fault)
void AssetManager::AppendToSearchPath(const char* path) {
	if (path && path[0])
		paths.push_back(path);
}

void AssetManager::ResetPaths() {
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
CString AssetManager::GetPathOf(const char* filename) const {
	
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
bool AssetManager::FileExists(const char* file) const {
	struct stat sb;
	
	if (stat(file, &sb) == -1)
		return false;
		
	return true;
}

//! Opens a bitmap, utilizes the search paths
BITMAP* AssetManager::LoadBitmap(const char* filename, PALETTE* pal) {
	BITMAP* bmp = NULL;
	
	// 1) See if this bitmap is already loaded
	BitmapListIter i = bitmaps.find(filename);

	if (i != bitmaps.end()) {
		return i->second;		// return the already loaded bitmap
	}

	// 2) Try to open the file
	CString file = GetPathOf(filename);
	if (file.length() != 0) {
		bmp = load_bitmap(file, *pal);

		// success, add it to the list
		if (bmp)
			bitmaps[filename] = bmp;
	}
	
	return bmp;
}

SAMPLE* AssetManager::LoadSound(const char* filename) {
	SAMPLE *spl = NULL;

	// 1) See if this sample is already loaded
	SampleListIter i = samples.find(filename);

	if (i != samples.end()) {
		return i->second;		// return the already loaded sample
	}

	// 2) Try to open the file
	CString file = GetPathOf(filename);
	if (file.length() != 0) {
		spl = load_sample(file);

		if (spl)
			samples[filename] = spl;
	}

	return spl;
}

AssetManager::AssetManager() {
	ResetPaths();
}

AssetManager::~AssetManager() {
	Shutdown();
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
CString AssetManager::GetMacOSXCurrentWorkingDir() const {
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

