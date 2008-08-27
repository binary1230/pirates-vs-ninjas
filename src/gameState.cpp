#include "stdafx.h"
#include "gameState.h"

// #include "gui.h"
#include "gameOptions.h"
#include "input.h"
#include "window.h"
#include "gameMode.h"
#include "assetManager.h"
#include "xmlParser.h"
#include "gameSound.h"
#include "gameModes.h"
#include "globalDefines.h"
#include "luaManager.h"
#include "mapEditor.h"
#include "physics.h"

DECLARE_SINGLETON(GameState)

void GameState::ResetAccumulatedTime() {
	g_iOutstanding_updates = 0;
}

// Parse the master XML file
// returns: XMLNode of first GameMode to load
int GameState::LoadXMLConfig(CString xml_filename) {
				
	// XXX xmlParser just DIES on error
	xml_filename = ASSETMANAGER->GetPathOf(xml_filename.c_str());
	xGame = XMLNode::openFileHelper(xml_filename.c_str(), "game");
	
	XMLNode xInfo = xGame.getChildNode("info");

	TRACE(
		" Mod Info: requires engine version '%s'\n"
		" Mod Info: map version '%s'\n"
		" Mod Info: map author '%s'\n"
		" Mod Info: Description: '%s'\n",
		// " Mod Info: Number of modes: '%i'\n"
		xInfo.getChildNode("requires_engine_version").getText(),
		xInfo.getChildNode("game_version").getText(),
		xInfo.getChildNode("author").getText(),
		xInfo.getText()
		//max, 
		);

	// Init globals XML stuff
	XMLNode xGlobalVars = xGame.getChildNode("global_vars");
	GLOBALS->CreateInstance();
	GLOBALS->Init(xGlobalVars);

	return 0;	
}

void GameState::SignalEndCurrentMode() {
	modes->SignalEndCurrentMode();
}

int allegro_debug_printer(const char *text)
{
	// filter out some spam here.
	if (strncmp("agl-tex INFO", text, 12) != 0 &&
		strncmp("agl-font INFO", text, 13) != 0 &&
		strncmp("agl-win INFO", text, 12) != 0 &&
		strncmp("agl-scorer INFO", text, 15) != 0 &&
		strncmp("al-gfx INFO", text, 11) != 0 )
	{
		        
		TRACE("%s", text);
	}

	return 1; // don't let allegro process this msg further, we handled it.
}

//! Initialize basic allegro library stuff
//! This must be called FIRST before ANY allegro stuff
int GameState::InitAllegro() {
	
	if (allegro_init() != 0) {
		TRACE("-- FATAL ERROR: Allegro_init() failed.\n");
		return -1;
	}
	
	register_trace_handler(allegro_debug_printer);
	
	// must be called SECOND
	if (InitTimers() < 0) {
		TRACE("-- FATAL ERROR: Can't init timers.\n");
		return -1;
	}

	// register PNG as allegro filetype (see loadpng.h)
	register_png_file_type();

	SetRandomSeed(42);	// for now, makes testing easier

	return 0;
}

//! Initialize game systems - main function

//! This is the first init function, it needs to initialize
//! Allegro, the window, the input subsystem, and the default game mode
//! BE CAREFUL, things need to be done IN ORDER here.
int GameState::InitSystem() {
		
		TRACE("[Beginning Game Init]\n");
				
		exit_game = false;
		is_playing_back_demo = false;
		debug_pause_toggle = false;

		TRACE("[init: allegro]\n");
		if (InitAllegro() < 0) {
			TRACE("ERROR: InitSystem: failed to init allegro!\n");
			return -1;
		}

		TRACE("[init: assetManager]\n");
		ASSETMANAGER->CreateInstance();
		if (!ASSETMANAGER || ASSETMANAGER->Init() < 0) {
			TRACE("ERROR: InitSystem: failed to create assetManager!\n");
			return -1;
		}

		ASSETMANAGER->AppendToSearchPath("data/");

		TRACE("[init: xml config]\n");

		// just DIES if it can't load this file (bad)
		if (LoadXMLConfig("default.xml") < 0) {
			TRACE("ERROR: Failed to parse default.xml");	
			return -1;
		}

		TRACE("[init: window]\n");
		WINDOW->CreateInstance();
		if ( !WINDOW ||	WINDOW->Init(screen_size_x, screen_size_y, 
										OPTIONS->IsFullscreen(), OPTIONS->GraphicsMode()) < 0 ) {
			TRACE("ERROR: InitSystem: failed to init window!\n");
			return -1;
		}

		TRACE("[init: input subsystem]\n");
		if (InitInput() == -1) {
			TRACE("ERROR: InitSystem: failed to init input subsystem!\n");
			return -1;
		}

		TRACE("[init: sound subsystem]\n");
		if (InitSound() == -1) {
			TRACE("ERROR: InitSystem: failed to init sound subsystem!\n");
		}

		TRACE("[init: embedded lua scripting]\n");
		LUA->CreateInstance();
		if ( !LUA || !LUA->Init() ) {
			TRACE("ERROR: InitSystem: failed to init lua scripting!\n");
			return -1;
		}
		
		/*TRACE("[init: gui manager]\n");
		GUI->CreateInstance();
		if ( !GUI || !GUI->Init() ) {
			TRACE("ERROR: InitSystem: failed to init gui!\n");
			return -1;
		}*/

		TRACE("[init: loading game modes]\n");
		if (LoadGameModes() == -1) {
			TRACE("ERROR: InitSystem: failed to init default game mode!\n");
			return -1;
		}
		
		TRACE("[init complete]\n");
				
		return 0;
}

int GameState::LoadGameModes() {
	modes = new GameModes();

	if (!modes)
		return -1;

	return modes->Init(xGame);
}
 
int GameState::InitNetwork() {	

#if 0 // TODO: Networking disabled (e.g. WAY broken, not even close yet)

	int ret;
	int port = OPTIONS->GetNetworkPortNumber();

	if (!OPTIONS->IsNetworkEnabled())
		return 0;

	network = new GameNetwork();
	if (!network)
		return -1;

	TRACE("NET: Port %i\n", port);
	
	if (OPTIONS->IsNetworkServer()) {
		ret = network->InitServer(port);
	} else {
		ret = network->InitClient(port, OPTIONS->GetNetworkServerName());
	}

	return ret;
#endif 

	return 0;
}

//! Init sound subsystem
//TODO if sound init fails, make it just keep going instead of erroring out.
int GameState::InitSound() {

	SOUND->CreateInstance();

	if (!SOUND) {
		TRACE(" Failed to create sound instance.\n");
		return -1;
	}

	if (!OPTIONS->SoundEnabled())
		TRACE(" Sound disabled.\n");

	if ( !SOUND || (SOUND->Init(OPTIONS->SoundEnabled()) == -1) ) {
		return -1;
	}
				
	return 0;
}

//! Init input subsystem
int GameState::InitInput() {
	INPUT->CreateInstance();
	
	if ( !INPUT || (INPUT->Init() == -1) ) {
		return -1;
	}

	return 0;
}

//! Init game timers
//! This MUST be called BEFORE any other allegro initializations.
int GameState::InitTimers() {
	TRACE("[Init: Timers]");
	install_timer();
	LOCK_VARIABLE(g_iOutstanding_updates);
	LOCK_VARIABLE(g_iTicks);
	LOCK_FUNCTION((void*)Timer);
	return install_int_ex(Timer, BPS_TO_TIMER(FPS));
}

void GameState::OutputTotalRunningTime() {
	int seconds_played = int((float)g_iTicks / (float)FPS);
	int seconds = seconds_played % 60;
	int minutes = seconds_played / 60;
	char* min_string = "minutes";

	if (minutes == 1) 
		min_string = "minute";

	fprintf(stderr, 
			"[You ninja'd in the night for %i %s and %.2i seconds]\n",
			minutes, min_string, seconds );
}



//! The 'main' function for the game

//! It takes a pointer to the game options (fullscreen/etc).
//! It initializes everything, and returns 0 if successful
//! or 1 on error.
int GameState::RunGame() {
	
		if (InitSystem() == -1) {
			TRACE("ERROR: Failed to init game!\n");
			return -1;	
		}

		if (OPTIONS->GetDebugStartPaused()) 
		debug_pause_toggle = 1;	

		INPUT->Begin();
			
		TRACE("[running game...]\n");
		MainLoop();
		TRACE("[done running game!]\n");

		OutputTotalRunningTime();

		INPUT->End();
	
		Shutdown();
		TRACE("[Exiting]\n");	

		return 0;
}

void GameState::MainLoop() 
{
	// NOT sure this is the best place for this:
	if (OPTIONS->MapEditorEnabled()) 
	{
		// pray to god.
		assert(WORLD);
		MapEditor* pkMapEditor = (MapEditor*)(WORLD);

		assert(pkMapEditor);
		pkMapEditor->RunMapEditor();
		return;
	}

	while (!exit_game) 
	{
		Tick();
	}
}

//! The most important function.  It will make sure that the game 
//! is updating at the correct speed, and it will Draw everything
//! at the correct speed.

// XXX NOTE: This is currently a bit more complex than it should be.
// If you are trying to understand it, ignore all the DEBUG_ and pause junk.
void GameState::Tick() 
{
	// outstanding_updates is incremented once every 1/30th of a sec.
	// We may need to update more than once on slower computers
	// before we can draw, in order to keep the game the same speed
	// no matter the speed of the computer
	while (g_iOutstanding_updates > 0 && !exit_game) 
	{
		// failsafe: If there are too many updates left, break out so we can draw.
		// this will slow the actual gamespeed down, but it will at least draw
		// You shouldn't see this unless either 1) something's wrong or 2) uber-slow computer
		if (m_iCurrentFps <= 2 || g_iOutstanding_updates > 10) 
		{
			g_iOutstanding_updates = 0;
			break;
		}

		Update();	// mode signals handled here

		UpdateDebugPausing();

		--g_iOutstanding_updates;
	}

	if (!exit_game) {
		Draw();

		if (INPUT->KeyOnce(GAMEKEY_SCREENSHOT) && !OPTIONS->MapEditorEnabled())
			WINDOW->Screenshot();

		if (INPUT->KeyOnce(GAMEKEY_TOGGLE_PHYSICS_DISPLAY) && !OPTIONS->MapEditorEnabled() && PHYSICS)
			PHYSICS->SetDrawDebug(!PHYSICS->GetDrawDebug());
	}

	// Normally, now that we're done one Tick, we wait around for the next 1/30th of a sec
	// to pass us by.
	//
	// However in some debugging situations, we don't want to wait, we instead just want to bolt through
	// the game as fast as possible (e.g. AI training, regression testing, fast forwarding, etc)
	if (!OPTIONS->WaitForUpdates()) 
		g_iOutstanding_updates = 1;

	// wait for 1/30th sec to elapse (if we're on a fast computer)
	// note: this should really be down() on a lock of some kind rather than
	// just sleep randomly.
	while (g_iOutstanding_updates <= 0 && !exit_game) {
		// rest(5);	// 1/30 sec is 33 usec, we sleep for 5 to be conservative
		// SPIN!! (not great..)
	}
}

//! Update all game status
void GameState::Update() {
	if (exit_game)
		return;

	SOUND->Update();
	INPUT->Update();
	// GUI->Update();
	WINDOW->Update(); // update fades.

	modes->Update();
}

void GameState::UpdateFPS()
{ 
	static int iTicksAtLastFrameDrawn = g_iTicks;
	static int iAmountOfFramesDrawnSinceLastCheck = 0;

	int iDiff = g_iTicks - iTicksAtLastFrameDrawn;

	if (iDiff > FPS) {

		// The new actual FPS rate for the last second:
		m_iCurrentFps = iAmountOfFramesDrawnSinceLastCheck;

		// TRACE("FPS: %d\n", m_iCurrentFps);

		iAmountOfFramesDrawnSinceLastCheck = 0;
		iTicksAtLastFrameDrawn = g_iTicks;
	} else {
		iAmountOfFramesDrawnSinceLastCheck++;
	}
}

//! Draw the current mode
void GameState::Draw() {

	UpdateFPS();

	if (OPTIONS->DrawGraphics()) {
		WINDOW->BeginDrawing();
		WINDOW->Clear();

		// Tell everything to draw itself
		modes->Draw();
		WINDOW->Draw();
		//GUI->Draw();

		WINDOW->Flip();
		WINDOW->EndDrawing();
	}
}

void GameState::Shutdown() {
	TRACE("[Shutting Down]\n");	

	if (INPUT) {
		INPUT->Shutdown();
		INPUT->FreeInstance();
	}

	remove_int(Timer);

#if 0 // TODO: Networking disabled (e.g. WAY broken, not even close yet)
	if (network) {
		network->Shutdown();
		delete network;
	}
#endif // WIN32

	if (modes) {
		modes->Shutdown();
		delete modes;
	}

	/*if (GUI) {
		GUI->Shutdown();
		GUI->FreeInstance();
	}*/

	if (LUA) {
		LUA->Shutdown();
	}

	if (SOUND) {
		SOUND->Shutdown();
		SOUND->FreeInstance();
	}

	if (ASSETMANAGER) {
		ASSETMANAGER->Shutdown();
		ASSETMANAGER->FreeInstance();
	}
	
	// window destruction code must be LAST
	if (WINDOW) {
		WINDOW->Shutdown();
		WINDOW->FreeInstance();
	}

	GLOBALS->Shutdown();

	modes = NULL;
	network = NULL;
	xGame = XMLNode::emptyXMLNode;
	
	allegro_exit();
}

void GameState::SetRandomSeed(int val) { 
	random_seed = val; 
	srand(val); 
};

int GameState::GetRandomSeed() const { 
	return random_seed; 
};

GameState::GameState() {
	modes = NULL;
	network = NULL;
}

void GameState::SignalGameExit() {
	exit_game = true; 
	is_playing_back_demo = false;
	modes->SignalGameExit();
}

GameState::~GameState() {}

void GameState::UpdateDebugPausing()
{
	if (INPUT->KeyOnce(GAMEKEY_DEBUGPAUSE) && !OPTIONS->MapEditorEnabled())
		debug_pause_toggle = !debug_pause_toggle;

	if (debug_pause_toggle)
	{
		int debug_update_count = g_iOutstanding_updates;

		while (debug_pause_toggle && !INPUT->KeyOnce(GAMEKEY_DEBUGSTEP)) 
		{
			INPUT->Update();
			SOUND->Update();

			Draw();

			if (INPUT->KeyOnce(GAMEKEY_SCREENSHOT))
				WINDOW->Screenshot();

			if (INPUT->KeyOnce(GAMEKEY_DEBUGPAUSE))
				debug_pause_toggle = !debug_pause_toggle;
		}

		g_iOutstanding_updates = debug_update_count;
	}
}

/*#define PVN_NETWORK_MAGIC_GREETING 123454321

int GameState::InitNetworkServer() {
	int port = OPTIONS->GetNetworkPortNumber();
  ezSocketsPacket packet;
	
	TRACE("NET: Starting UDP network server on port %i\n", port);
	
  socket->mode = ezSockets::skUDP;
  socket->Create(IPPROTO_UDP, SOCK_DGRAM);
  socket->Bind(port);
	
	TRACE("NET: Waiting for client greeting..\n");

	bool got_greeting = false;

	while (!got_greeting) {
		rest(100);
		if (socket->ReadPack(packet)) {
			int size = packet.Read4();
      if (size != packet.Size-4)
        TRACE("NET: WARN: Merged packets!\n");

			// Expect MAGIC greeting from client
			if (packet.Read4() != PVN_NETWORK_MAGIC_GREETING) {
				TRACE("Incorrect MAGIC recieved from client, aborting!\n");
				return -1;	
			} else {
				got_greeting = true;
			}
    }
	}

	rest(1000);
	packet.ClearPacket();

	// Send MAGIC greeting to client
  packet.Write4(PVN_NETWORK_MAGIC_GREETING);
	socket->SendPack(packet);

	TRACE("NET: Server: Connected to client OK!\n");

	return 0;
}*/


