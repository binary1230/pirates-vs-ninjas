#ifndef GAMESOUND_H
#define GAMESOUND_H

#include "globals.h"
#include "gameBase.h"
#include "xmlParser.h"
#include "StdString.h"

#include <allegro.h>
#include <stdio.h>
#include <map>

using namespace std;

//! Maps a sound name to it's handle (e.g. "jump" to the sample)
typedef map<CString, SAMPLE*> SoundMapping;
typedef map<CString, SAMPLE*>::iterator s_iter;

class GameSound : public GameBase {
				
		protected:
			//! If false, then this class doesn't output any sound
			bool sound_enabled;

			//! Holds pointers to SAMPLE's.  Do NOT create or free these
			//! pointers, AssetManager does that.
			SoundMapping sounds;
						
		public:
			//! Load a sound
			//! example: LoadSound("sounds/jump.wav", "jump")
			//! Then you can PlaySound("jump") to hear it.  Neat eh?
			bool LoadSound(const char* filename, const char* sound_name);
			
			//! Load all sounds from a <sounds> block in XML
			bool LoadSounds(XMLNode &xSounds);
			
			void PlaySound(CString name, unsigned int pan = 128);

			//! Init the sound system
			int Init(GameState* _game_state, bool _sound_enabled);

			//! Shutdown the sound system
			void Shutdown();

			GameSound();
			~GameSound();
};

#endif // GAMESOUND_H
