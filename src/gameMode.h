#ifndef GAMEMODE_H
#define GAMEMODE_H

//! Holds info about the last mode completed (if wanted)
struct GameModeExitInfo {

	bool useExitInfo;					//! Whether to use any of this info at all.

	std::string lastModeName;			//! The name of the mode that is exiting 
														//! (e.g. "level1.xml") 

	std::string nextModeToLoad;		//! If not empty, holds the name of the
														//! next mode to run. (e.g. "level2.xml")

	std::string lastPortalName;		//! The last portal we went through
														//! (allows player to spawn at a specific door)
	
	bool useNextModeToLoad;
	bool useLastPortalName;

	bool showInitialText;			//! sorta-hack.  set to false to disable initial
														//! text.  useful for portal warping

	GameModeExitInfo() {
		nextModeToLoad = lastPortalName = lastModeName = "";
		useExitInfo = useNextModeToLoad = useLastPortalName = false;
		showInitialText = true;
	}
};

//! Base class for all Modes of the game (e.g. Menu Mode, Game Mode, Bonus Level Mode)

//! For example, a level is a "mode", the menu screen is a "mode"
//! and the bonus stage is a "mode"
//! 
//! This class doesn't do anything except act as an interface to enforce
//! the fact that all modes need to be able to Update() and Draw(), among
//! other things.  It also acts as a node in a linked list in order to hold
//! all the game modes.
class GameMode {

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// implement if needed
	}

		protected:

			// our exit info that we will return 
			// upon ending this mode
			GameModeExitInfo exitInfo;				

			// the exit info the last mode returned
			// or NULL if there was none
			GameModeExitInfo lastExitInfo;		

		public:
			virtual int Init(XMLNode) = 0;
			virtual void Shutdown() = 0;

			//! Draw this mode
			virtual void Draw() = 0;

			//! Update this mode
			virtual void Update() = 0;

			//! Experimental: get the AI fitness score for AI training	
			virtual int GetAiFitnessScore() {return 0;}

			//! Get the exit info from this mode
			//! NOTE: Not all modes return meaningful exit info
			void SetExitInfo(const GameModeExitInfo& info) {exitInfo = info;}
			const GameModeExitInfo& GetExitInfo() {return exitInfo;}

			//! Return the exit info from the LAST mode, NOT us. NULL if none exists
			void SetOldExitInfo(const GameModeExitInfo& info) {lastExitInfo = info;}
			const GameModeExitInfo& GetOldExitInfo() {return lastExitInfo;}

			GameMode();
			virtual ~GameMode();
};

#endif // GAMEMODE_H
