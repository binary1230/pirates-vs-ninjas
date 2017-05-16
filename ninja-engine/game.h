#ifndef GAME_H
#define GAME_H

class Game;
class BaseInput;
class GameMode;
class AssetManager;
class GameSound;
class GameModes;
class GameState;

//! The MAIN class in the game - the centrally
//! coordinated object which initializes, runs, and destroys
//! the entire game.  Anything of importance starts in this
//! class somewhere.
class Game {
	DECLARE_SINGLETON_CLASS(Game)

	protected:
		ALLEGRO_TIMER* m_timer;

		bool should_redraw;

		//! Holds the current game's parsed XML data
		XMLNode xGame;
		
		//! Collection of all game modes (e.g. menu mode, simulation mode, etc)
		GameModes *modes;

		//! Fires off timer related events
		ALLEGRO_EVENT_QUEUE *event_queue = NULL;
		
		//! Initialize all game related stuff
		bool InitSystems();								
		
		//! Init basic allegro stuff
		bool InitAllegro();

		//! Init game timing
		bool InitAllegroEvents();

		void InitProfiler();
		
		//! Init input subsystems
		bool InitInput();								

		//! Init sound system
		bool InitSound();

		//! Load master game config from an XML file, return an XMLNode which is
		//! the first game Mode's config we should now load.
		bool LoadXMLConfig(std::string xml_filename);
		
		//! Load a game mode from an XML config file
		bool LoadGameModes();

		//! Update the state of the game
		void Update();

		//! Draw the current state of the game
		void Draw();

		//! Set to TRUE to immediately shutdown the game 
		bool exit_game;	
	
		//! Flag to enable DEBUG pause toggling
		bool paused;
		bool unpause_only_this_frame;

		//! If a game has the same random seed, it will
		//! return the EXACT same sequence of random numbers.
		//! (useful for recording demos)
		int random_seed;
	
		//! Set to true if we are playing back a demo
		bool is_playing_back_demo;

		//! The current FPS rate
		int m_iCurrentFps;

		GameState* _state;
					
	public:

		//! Shutdown the game
		void Shutdown();

		//! Initialize
		bool Init(const int argc, const char** argv);

		//! THE MAIN LOOP
		void RunMainLoop_BlockingHelper();

		void ProcessEvents();

		void TickIfNeeded();

		//! NOTE: Normally called by MainLoop()
		//! Sometimes GUI's and things will have to call this directly from ON_IDLE msgs
		void Tick();

		void UpdateGlobalInput();

		//! Set the random seed value
 		void SetRandomSeed(int);

		inline bool IsPaused() {
			return paused;
		}

		inline void SetPaused(bool newState) {
			paused = newState;
		}
		
		//! Get the random seed value
		int GetRandomSeed() const;

		//! Returns true if we are playing back a demo
		bool IsPlayingBackDemo() {return is_playing_back_demo;};

		//! Unconditionally exit game
		void SignalGameExit();
		
		//! The current mode calls this to signal it wants to end
		void SignalEndCurrentMode();

		bool ShouldExit() { return exit_game; }

		void CreateGameStateIfNotExists();
		void StartNewGame();
		void FreeGameState();

		GameState* GetState();
		
		~Game();

		CREATE_PROPERTY(bool, PhysicsDebugDraw)
};

#define GAME Game::GetInstance()

#endif // GAME_H