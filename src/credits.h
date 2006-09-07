#ifndef CREDITS_H
#define CREDITS_H

#include "stdafx.h"
#include "gameBase.h"
#include "gameMode.h"

class GameState;
class OGGFILE;

//! Represents a physical simulation (the main game levels)
class CreditsMode : public GameMode {
		protected:		
			BITMAP* credits_bmp;
			int scroll_speed;
			int scroll_offset;
			OGGFILE* music;

		public:
			int Init(GameState*, XMLNode);
			void Shutdown();

			void Draw();
			void Update();

			CreditsMode();
			~CreditsMode();
};

#endif
