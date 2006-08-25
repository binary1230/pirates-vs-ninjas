#ifndef GAME_MUSIC_H
#define GAME_MUSIC_H

#include "alogg.h"
#include "allegro_wrapper.h"

#include "gameBase.h"
#include "StdString.h"

class GameMusic : public GameBase {
	protected:
		bool song_is_loaded;
		bool is_paused;
		
	public:
		void Update();

		bool LoadSong(CString name);

		void Pause();					// Pause the song playing
		void PauseToggle();
		void Resume();
		void Stop();

		//! Init the sound system
		int Init(GameState* _game_state);

		//! Shutdown the sound system
		void Shutdown();

		GameMusic();
		~GameMusic();
};

#endif // GAME_MUSIC_H
