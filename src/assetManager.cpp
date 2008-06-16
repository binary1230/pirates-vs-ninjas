#include "stdafx.h"
#include "assetManager.h"
#include "oggFile.h"
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
	FreeMusic();
}

// XXX should make these templated...
void AssetManager::FreeSamples() {
	SampleListIter i;
	for (i = samples.begin(); i != samples.end(); i++) {
		if (i->second)
			destroy_sample(i->second);
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
	}
	*/
	sprites.clear();
}

/*void AssetManager::FreeSongs() {
	SongListIter i;
	for (i = songs.begin(); i != songs.end(); i++) {
		if (i->second)
			(i->second);
	}
	songs.clear();
}*/

void AssetManager::FreeMusic() {
	if (music) {
		music->Shutdown();
		delete music;
		music = NULL;
	}
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
// XXX: Need to fix alpha blending
Sprite* AssetManager::LoadSprite(	const char* filename, 
									bool use_alpha, 
									PALETTE* pal) 
{	
	Sprite* sprite = NULL;
	
	int original_bpp = get_color_depth();
	
	// 1) See if this bitmap is already loaded
	SpriteListIter i = sprites.find(filename);

	if (i != sprites.end()) {
		return i->second;		// return the already loaded bitmap
	}

	// 2) Try to open the file
	CString file = GetPathOf(filename);
	if (file.length() != 0) {

		sprite = new Sprite();
		assert(sprite && "ERROR: Out of memory, can't allocate sprite!\n");
	
		if (use_alpha) {
			set_color_depth(32);
		}
			
		BITMAP* bmp = load_bitmap(file, *pal);
	
		if (!bmp) {
			TRACE("ERROR: Can't load bitmap file: '%s'\n", file.c_str());
			delete sprite;
			return NULL;
		}

		sprite->width = bmp->w;
		sprite->height = bmp->h;

		// make the OpenGL texture
		// this makes a copy of the bitmap
		if (!use_alpha)
			sprite->texture = allegro_gl_make_masked_texture(bmp);
		else
			sprite->texture = allegro_gl_make_texture_ex(
				AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, bmp, GL_RGBA
			);
				
		// don't need the original Allegro bitmap anymore
		destroy_bitmap(bmp);
		bmp = NULL;
		
		set_color_depth(original_bpp);

		// add to the loaded sprites list
		if (sprite->texture != 0) {
			sprites[filename] = sprite;
		} else {
			TRACE(	"ERROR: Failed making texture for '%s'\n"
												"-NOTE: Make sure texture size is a multiple of 2!\n",
												file.c_str());

			if (allegro_gl_error && strlen(allegro_gl_error))
				TRACE("       AllegroGL says: %s\n", allegro_gl_error);

			delete sprite;
			return NULL;
		}
	}
	
	return sprite;
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

OGGFILE* AssetManager::LoadMusic(const char* filename) {
	CString music_file = GetPathOf(filename);

	if (music_file.GetLength() < 0) {
		TRACE(" - WARN: Can't find music file: %s\n", filename);
		return 0;
	}

	if (music) {
		music->Shutdown();
		music = NULL;
	}

	music = new OGGFILE();

	if (!music) {
		TRACE(" - ERROR: Out of memory while trying to load %s!\n", filename);
		return NULL;
	}
	
	if (!music->Init(music_file) ) {
		TRACE(" - WARN: Invalid music file: %s\n", filename);
		music->Shutdown();
		return NULL;
	}

	return music;
}

AssetManager::AssetManager() {
	ResetPaths();
	music = NULL;
}

AssetManager::~AssetManager() {
	Shutdown();
	music = NULL;
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
	TRACE("Assetmanager: Using MacOSX Carbon stuff.\n");
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyBundleURL(mainBundle);
	CFStringRef cfStr = CFURLCopyPath(url);
	CString path = CFStringGetCStringPtr(cfStr, CFStringGetSystemEncoding());
	TRACE("Assetmanager: Adding path: '%s'\n", path.c_str());
	return CString(path + "Resources/");
#else
	return "";
#endif // PLATFORM_DARWIN
}

