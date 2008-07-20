#ifndef GAMESOUND_H
#define GAMESOUND_H

//! Maps a sound name to it's handle (e.g. "jump" to the sample)
typedef map<CString, SAMPLE*> SoundMapping;
typedef map<CString, SAMPLE*>::iterator s_iter;

class OGGFILE;

class GameSound {

		DECLARE_SINGLETON_CLASS(GameSound)
				
		protected:
			//! If false, then this class doesn't output any sound
			bool sound_enabled;

			//! Holds pointers to SAMPLE's.  Do NOT create or free these
			//! pointers, AssetManager does that.
			SoundMapping soundMap;

			//! Pitch altering params
			bool use_variable_pitch;
			int freq_range;

		public:
			//! Load a sound
			//! example: LoadSound("sounds/jump.wav", "jump")
			//! Then you can PlaySound("jump") to hear it.  Neat eh?
			bool LoadSound(const char* filename, const char* sound_name);
	
			//! Load all sounds from a <sounds> block in XML
			bool LoadSounds(XMLNode &xSounds);
			
			//! Load a piece of music
			bool LoadMusic(const char* filename);

			//! Plays the currently loaded song
			bool PlayMusic( bool loop=false, int vol=255, int pan=128, 
											int buflen=DEFAULT_MUSIC_BUFFER_SIZE);

			void PauseMusic();					// If playing, pause the song
			void PauseToggleMusic();		// If playing pause the song, resume if not playing
			void ResumeMusic();					// If not playing, resume the song
			void StopMusic();						// Stop the music
			
			void PlaySound(CString name, unsigned int pan = 128);

			//! Init the sound system
			int Init(bool _sound_enabled);

			void Update();

			//! Shutdown the sound system
			void Shutdown();

			void ClearSoundMap();

			~GameSound();
};

#define SOUND GameSound::GetInstance()

#endif // GAMESOUND_H
