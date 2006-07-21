#ifndef GAMESOUND_H
#define GAMESOUND_H

#include "globals.h"
#include "gameBase.h"
#include "xmlParser.h"
#include "StdString.h"

#include <allegro.h>
#include <stdio.h>
#include <vector>

using namespace std;

class GameSound : public GameBase {
				
		protected:
			//! If false, then this class doesn't output any sound
			bool sound_enabled;

			//! Holds the sounds that are loaded
			vector<SAMPLE*> sounds;
						
		public:
			int LoadSound(const char* filename);
			
			void PlaySound(unsigned int which_sound, unsigned int pan = 128);

			//! Init the sound system
			int Init(GameState* _game_state, bool _sound_enabled);

			//! Shutdown the sound system
			void Shutdown();

			GameSound();
			~GameSound();
};

#endif // GAMESOUND_H
