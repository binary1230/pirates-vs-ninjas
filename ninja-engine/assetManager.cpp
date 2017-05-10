#include "stdafx.h"
#include "assetManager.h"
#include "sprite.h"
#include "gameSound.h"

DECLARE_SINGLETON(AssetManager)

int AssetManager::Init() {
	ResetPaths();
	sprites.clear();
	samples.clear();
	return true;
}

void AssetManager::Free() {
	FreeSprites();
	FreeSamples();
}

string AssetManager::GetCurrentWorkingDir() {
	char cCurrentPath[FILENAME_MAX];
	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath))) {
		return "";
	}

	return string(cCurrentPath);
}

string AssetManager::GetCurrentExeFullPath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return string(buffer);
}

// XXX should make these templated...
void AssetManager::FreeSamples() {
	for (auto i = samples.begin(); i != samples.end(); i++) {
		if (i->second)
			al_destroy_sample(i->second);
	}
	samples.clear();
	if (SOUND)
		SOUND->ClearSoundMap();
}

void AssetManager::FreeSprites() {
	/*SpriteListIter i;
	Sprite* sprite;
	for (i = sprites.begin(); i != sprites.end(); i++) {
		
		sprite = i->second;

		if (!sprite)
			continue;

		// TODO: probably something in here about freeing GL textures
		// 2017 update: not sure any of this is needed anymore.
	}
	*/
	sprites.clear();
}

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
	std::string MacOSXWorkingDir = GetMacOSXCurrentWorkingDir();
	if (MacOSXWorkingDir != "") {
		paths.push_back(MacOSXWorkingDir);
	}
	
	// For everyone else..
	paths.push_back("./");
}

//! Returns either the full path to a real file,
//! or an empty std::string
std::string AssetManager::GetPathOf(const char* filename) const {
	
	const std::string seperator = "/";
	std::string fullpath;
				
	for (uint i = 0; i < paths.size(); i++) {
		fullpath = paths[i] + seperator + filename;
		if (FileExists(fullpath.c_str()))
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
Sprite* AssetManager::LoadSprite(const char* filename, bool suppress_file_errors) 
{	
	Sprite* sprite = NULL;
	
	// 1) See if this bitmap is already loaded
	auto i = sprites.find(filename);

	if (i != sprites.end()) {
		return i->second;		// return the already loaded bitmap
	}

	// 2) Try to open the file
	std::string file = GetPathOf(filename);
	if (file.length() != 0) {
		al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);

		int flags = al_get_new_bitmap_flags();
		flags |= ALLEGRO_NO_PREMULTIPLIED_ALPHA;
		al_set_new_bitmap_flags(flags);
			
		ALLEGRO_BITMAP* bmp = al_load_bitmap(file.c_str());

		// backwards-comaptibility:
		// old versions of allegro would use magenta as transparent
		// these days, we can just use real alpha channels becuase it's not a 2006 DOS game.
		// re-enable this if you're using old assets
		#ifdef OLD_ASSET_MAGENTA_MASK_SUPPORT
		al_convert_mask_to_alpha(bmp, al_map_rgb(255, 0, 255));
		#endif
	
		if (!bmp) {
			if (!suppress_file_errors) {
				TRACE("ERROR: Can't load bitmap file: '%s'\n", file);
			}
			return NULL;
		}

		uint opengl_tex_id = al_get_opengl_texture(bmp);

		if (opengl_tex_id == 0) {
			TRACE("ERROR: Failed making texture for '%s'\n", file.c_str());
			return NULL;
		}

		sprite = new Sprite();

		sprite->width = al_get_bitmap_width(bmp);
		sprite->height = al_get_bitmap_height(bmp);
		sprite->texture = opengl_tex_id;

		sprites[filename] = sprite;

		bmp = NULL;
	}
	
	return sprite;
}

ALLEGRO_SAMPLE* AssetManager::LoadSound(const char* filename) {
	ALLEGRO_SAMPLE *spl = NULL;

	// 1) See if this sample is already loaded
	auto i = samples.find(filename);

	if (i != samples.end()) {
		return i->second;		// return the already loaded sample
	}

	// 2) Try to open the file
	std::string file = GetPathOf(filename);
	if (file.length() != 0) {
		spl = al_load_sample(file.c_str());

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
std::string AssetManager::GetMacOSXCurrentWorkingDir() const {
#ifdef PLATFORM_DARWIN 
	TRACE("Assetmanager: Using MacOSX Carbon stuff.\n");
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyBundleURL(mainBundle);
	CFStringRef cfStr = CFURLCopyPath(url);
	std::string path = CFStringGetstd::stringPtr(cfStr, CFStringGetSystemEncoding());
	TRACE("Assetmanager: Adding path: '%s'\n", path.c_str());
	return std::string(path + "Resources/");
#else
	return "";
#endif // PLATFORM_DARWIN
}

