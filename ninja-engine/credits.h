#ifndef CREDITS_H
#define CREDITS_H

#include "gameMode.h"
#include "sprite.h"

class Game;
class OGGFILE;

//! Represents a physical simulation (the main game levels)
class CreditsMode : public GameMode {
		protected:		
			Sprite* credits_sprite;
			int scroll_speed;
			int scroll_offset;
			OGGFILE* music;

		public:
			bool Init(XMLNode);
			void Shutdown();

			void Draw();
			void Update();

			CreditsMode();
			~CreditsMode();
};

#endif
