#ifndef OGGFILE_H
#define OGGFILE_H

#include <allegro.h>
#include "alogg.h"

#define DATASZ  (1<<15) // (32768) amount of data to read from disk each time 
#define BUFSZ   (1<<16) // (65536) size of audiostream buffer 

class OGGFILE {
	protected:
	  PACKFILE *f;
	  ALOGG_OGGSTREAM *s;

		int Poll();

	public:
		bool Open(const char* file);
		bool Play(bool loop=false, int vol=255, int pan=128, int buflen=BUFSZ);
		void Close();

		void Update();

		OGGFILE();
		~OGGFILE();
};

#endif // OGGFILE_H
