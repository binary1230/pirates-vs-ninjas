#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "globals.h"

/* The primary things the Asset manager does are this:
 * 
 * 1) Resolve paths
 *    It resolves relative paths to absolute paths, and handles
 *    platform specific path weirdness (as we have on MacOSx)
 *
 * 2) Handle duplicate assets (bitmaps/sounds/etc) efficiently
 * 		Example: if something asks for "ring.bmp", we load it the first time,
 * 		         we store it in sprites["ring.bmp"]. The second time something 
 * 		         asks for "ring.bmp", we find the copy we already loaded, and
 * 		         use that.
 *
 * All deletion/creation of assets are handled HERE and only HERE.
 */

class Sprite;

typedef map<std::string, Sprite*> SpriteList;
typedef map<std::string, Sprite*>::iterator SpriteListIter;

class SoundDef {
	public:
		ALLEGRO_SAMPLE* _sample;
		bool _is_resident;			// resident samples load at start, then stay loaded forever
};

typedef map<std::string, SoundDef> SampleList;

//! Manages game assets and memory
class AssetManager {

	DECLARE_SINGLETON_CLASS(AssetManager)

	protected:
		vector<std::string> paths;
		SpriteList sprites;
		SampleList samples;

		SoundDef* LoadSampleFromFile(const char * filename, bool is_resident);
		
	public:
		int Init();
		void Shutdown();

		void FreeSprites();
		void FreeSamples(bool keep_resident_sounds = true);
		void Free(bool free_resident_data = false);

		static string GetCurrentExeFullPath();
		static string GetCurrentWorkingDir();

		//! Append a new path to the search path
		void AppendToSearchPath(const char* path);

		//! Reset search paths
		void ResetPaths();

		//! This function either returns a full to a file path which 
		//! is guaranteed to exist, or returns "" if one can't be found
		//! in the current search path
		std::string GetPathOf(const char* filename) const;

		//! Returns true if the file exists.
		bool FileExists(const char* file) const;

		//! Opens a bitmap file, or returns NULL on failure
		//! This function looks in the current search path
		Sprite* LoadSprite(const char* filename, bool suppress_file_errors = false);

		//! Opens a sound file (e.g. WAV), or returns NULL on failure
		//! This function looks in the current search path
		SoundDef* LoadSound(const char* filename, bool load_resident = false);

		//! Returns the current working directory
		std::string GetMacOSXCurrentWorkingDir() const;

		virtual ~AssetManager();
};

#define ASSETMANAGER AssetManager::GetInstance()

#endif // ASSET_MANAGER_H
