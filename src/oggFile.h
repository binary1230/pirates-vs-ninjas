#ifndef OGGFILE_H
#define OGGFILE_H

#include <allegro.h>
#include "alogg.h"
#include "globals.h"

class OGGFILE {
	protected:
	  PACKFILE *f;
	  ALOGG_OGGSTREAM *s;

		int Poll();

		bool initialized;

	public:
		bool Init(const char* file);
		bool Play(bool loop=false, int vol=255, int pan=128, int buflen=DEFAULT_MUSIC_BUFFER_SIZE);
		void Shutdown();

		void Update();

		OGGFILE();
		~OGGFILE();
};

#endif // OGGFILE_H
