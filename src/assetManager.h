#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <map>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef WIN32
#include <unistd.h>
#endif

#include "allegro_wrapper.h"
#include "stl_wrapper.h"

#include "alogg.h"
#include "gameBase.h"
#include "StdString.h"
#include "globals.h"

/* The primary things the Asset manager does are this:
 * 
 * 1) Resolve paths
 *    It resolves relative paths to absolute paths, and handles
 *    platform specific path weirdness (as we have on MacOSx)
 *
 * 2) Handle duplicate assets (bitmaps/sounds/etc) efficiently
 * 		Example: if something asks for "ring.bmp", we load it the first time,
 * 		         we store it in bitmaps["ring.bmp"]. The second time something 
 * 		         asks for "ring.bmp", we find the copy we already loaded, and
 * 		         use that.
 *
 * All deletion/creation of assets are handled HERE and only HERE.
 */

typedef std::map<CString, BITMAP*> BitmapList;
typedef std::map<CString, BITMAP*>::iterator BitmapListIter;

typedef std::map<CString, SAMPLE*> SampleList;
typedef std::map<CString, SAMPLE*>::iterator SampleListIter;

//typedef std::map<CString, OGG*> SongList;
//typedef std::map<CString, OGG*>::iterator SongListIter;

//! Manages game assets and memory
class AssetManager : public GameBase {
	protected:
		std::vector<CString> paths;
		BitmapList bitmaps;
		SampleList samples;
		// SongList   songs;
		
	public:
		int Init(GameState*);
		void Shutdown();

		void FreeBitmaps();
		void FreeSamples();
		void Free();

		//! Set the search path
		void SetSearchPath(const char* path, ...);

		//! Append a new path to the search path
		void AppendToSearchPath(const char* path);

		//! Reset search paths
		void ResetPaths();

		//! This function either returns a full to a file path which 
		//! is guaranteed to exist, or returns "" if one can't be found
		//! in the current search path
		CString GetPathOf(const char* filename) const;

		//! Returns true if the file exists.
		bool FileExists(const char* file) const;

		//! Opens a bitmap file, or returns NULL on failure
		//! This function looks in the current search path
		//! it also outputs the palette in *pal
		BITMAP* LoadBitmap(const char* filename, PALETTE* pal = NULL); 

		//! Opens a sound file (e.g. WAV), or returns NULL on failure
		//! This function looks in the current search path
		SAMPLE* LoadSound(const char* filename);

		//! Opens an ogg file, or returns NULL on failure
		//! This function looks in the current search path
		// OGG* LoadSong(const char* filename);


		//! Returns the current working directory
		CString GetMacOSXCurrentWorkingDir() const;

		AssetManager();
		~AssetManager();
};

#endif // ASSET_MANAGER_H
