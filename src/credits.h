#ifndef CREDITS_H
#define CREDITS_H

#include "stdafx.h"
#include "singleton.h"
#include "gameMode.h"
#include "sprite.h"

class GameState;
class OGGFILE;

//! Represents a physical simulation (the main game levels)
class CreditsMode : public GameMode {
		protected:		
			Sprite* credits_sprite;
			int scroll_speed;
			int scroll_offset;
			OGGFILE* music;

		public:
			int Init(XMLNode);
			void Shutdown();

			void Draw();
			void Update();

			CreditsMode();
			~CreditsMode();
};

#endif
